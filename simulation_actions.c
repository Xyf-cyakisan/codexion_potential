/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:28:15 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/07 16:33:31 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_bool	thread_sleep(t_coder *coder)
{
	uint64_t	end_time;
	t_bool		appro_time;

	if (coder->status == COMPILING)
		appro_time = coder->time_compile;
	else if (coder->status == DEBUGING)
		appro_time = coder->time_debug;
	else
		appro_time = coder->time_refactor;
	end_time = true_get_time_of_day() + appro_time;
	while (true_get_time_of_day() < end_time)
	{
		usleep(100);
		if (simulation_is_stopped(coder) == TRUE)
			return (FALSE);
	}
	return (TRUE);
}

static void	compile(t_coder *coder, uint64_t time_start_of_simu)
{
	if (wait_for_cond(coder) == FALSE)
		return ;
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	wait_for_dongle_cd(coder);
	pthread_mutex_lock(&coder->dongle_1->heap.heap_mutex);
	pthread_mutex_lock(&coder->dongle_2->heap.heap_mutex);
	heap_pop(&coder->dongle_1->heap);
	heap_pop(&coder->dongle_2->heap);
	pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
	pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
	if (update_last_compile(coder) == FALSE)
	{
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		return ;
	}
	print_compile_log(coder, time_start_of_simu);
	if (thread_sleep(coder) == FALSE)
	{
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		return ;
	}
	update_required_compilations(coder);
	update_dongle_cooldown(coder);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	cond_broadcast(coder->cond, coder->cond_mutex);
}

static void	debug(t_coder *coder, uint64_t time_start_of_simu)
{
	print_log("is debugging\n", time_start_of_simu, coder);
	if (thread_sleep(coder) == FALSE)
		return ;
}

static void	refactor(t_coder *coder, uint64_t time_start_of_simu)
{
	print_log("is refactoring\n", time_start_of_simu, coder);
	if (thread_sleep(coder) == FALSE)
		return ;
}

void	coder_act(t_coder *coder, uint64_t time_start_of_simu,
			int required_comps_beg, t_request request)
{
	if (coder->nb_coders == 1)
		usleep(1000);
	else if (coder->status == COMPILING)
	{
		if (required_comps_beg > get_required_compilations(coder))
		{
			pthread_mutex_lock(&coder->dongle_1->heap.heap_mutex);
			pthread_mutex_lock(&coder->dongle_2->heap.heap_mutex);
			request = new_request(coder);
			heap_add_back(&coder->dongle_1->heap, request);
			heap_add_back(&coder->dongle_2->heap, request);
			pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
			pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
		}
		compile(coder, time_start_of_simu);
	}
	else if (coder->status == DEBUGING)
		debug(coder, time_start_of_simu);
	else if (coder->status == REFACTORING)
		refactor(coder, time_start_of_simu);
	if (coder->nb_coders != 1)
		coder->status = get_next_step(coder->status);
}

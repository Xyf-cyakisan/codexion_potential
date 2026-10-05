/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:28:15 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/05 23:51:14 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_bool	compile_sleep(t_coder *coder)
{
	int	counter;

	counter = 0;
	while (counter < 10)
	{
		usleep((coder->time_compile / 10) * 1000);
		if (simulation_is_stopped(coder) == TRUE)
		{
			pthread_mutex_unlock(&coder->dongle_1->mutex);
			pthread_mutex_unlock(&coder->dongle_2->mutex);
			return (FALSE);
		}
		++counter;
	}
	return (TRUE);
}

static void	compile(t_coder *coder, uint64_t time_start_of_simu)
{
	if (check_if_first(coder) == FALSE)
		return ;
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	if (check_if_coder_can_compile(coder) == FALSE)
		return ;
	pthread_mutex_lock(&coder->dongle_1->heap.heap_mutex);
	pthread_mutex_lock(&coder->dongle_2->heap.heap_mutex);
	heap_pop(&coder->dongle_1->heap);
	heap_pop(&coder->dongle_2->heap);
	pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
	pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
	print_log("has taken a dongle\n", time_start_of_simu, coder, FALSE);
	print_log("has taken a dongle\n", time_start_of_simu, coder, FALSE);
	pthread_mutex_lock(&coder->compile_mutex);
	coder->last_compile = true_get_time_of_day();
	pthread_mutex_unlock(&coder->compile_mutex);
	print_log("is compiling\n", time_start_of_simu, coder, FALSE);
	if (compile_sleep(coder) == FALSE)
		return ;
	update_required_compilations(coder);
	update_dongle_cooldown(coder);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	coder->status = get_next_step(coder->status);
}

static void	debug(t_coder *coder, uint64_t time_start_of_simu)
{
	print_log("is debugging\n", time_start_of_simu, coder, FALSE);
	usleep(1000 * coder->time_debug);
	coder->status = get_next_step(coder->status);
}

static void	refactor(t_coder *coder, uint64_t time_start_of_simu)
{
	print_log("is refactoring\n", time_start_of_simu, coder, FALSE);
	usleep(1000 * coder->time_refactor);
	coder->status = get_next_step(coder->status);
}

void	coder_act(t_coder *coder, uint64_t time_start_of_simu,
			int required_comps_beg, t_request request)
{
	if (coder->status == COMPILING && coder->nb_coders != 1)
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
		while (coder->status == COMPILING
			&& simulation_is_stopped(coder) == FALSE)
		{
			compile(coder, time_start_of_simu);
			usleep(10);
		}
	}
	else if (coder->status == DEBUGING)
		debug(coder, time_start_of_simu);
	else if (coder->status == REFACTORING)
		refactor(coder, time_start_of_simu);
}

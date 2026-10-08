/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:28:15 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 19:31:05 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	thread_sleep(t_coder *coder, uint64_t end_time)
{
	while (true_get_time_of_day() < end_time)
	{
		usleep(100);
		if (simulation_is_stopped(coder) == TRUE)
			return (FALSE);
	}
	return (TRUE);
}

static t_bool	compile(t_coder *coder, uint64_t time_start_of_simu)
{
	if (wait_for_cond(coder) == FALSE)
		return (FALSE);
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	if (deal_with_dongle_cd(coder) == FALSE)
	{
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		return (FALSE);
	}
	pthread_mutex_lock(&coder->dongle_1->heap.heap_mutex);
	pthread_mutex_lock(&coder->dongle_2->heap.heap_mutex);
	heap_pop(&coder->dongle_1->heap);
	heap_pop(&coder->dongle_2->heap);
	pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
	pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
	if (real_compile(coder, time_start_of_simu) == FALSE)
		return (FALSE);
	update_required_compilations(coder);
	update_dongle_cooldown(coder);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	cond_broadcast(coder->cond, coder->cond_mutex);
	return (TRUE);
}

static t_bool	debug(t_coder *coder, uint64_t time_start_of_simu)
{
	print_log("is debugging\n", time_start_of_simu, coder);
	if (thread_sleep(coder,
			true_get_time_of_day() + coder->time_debug) == FALSE)
		return (FALSE);
	return (TRUE);
}

static t_bool	refactor(t_coder *coder, uint64_t time_start_of_simu)
{
	print_log("is refactoring\n", time_start_of_simu, coder);
	if (thread_sleep(coder,
			true_get_time_of_day() + coder->time_refactor) == FALSE)
		return (FALSE);
	return (TRUE);
}

t_bool	coder_act(t_coder *coder, uint64_t time_start_of_simu,
			int required_comps_beg, t_request request)
{
	if (coder->status == COMPILING)
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
		if (compile(coder, time_start_of_simu) == FALSE)
			return (FALSE);
	}
	else if (coder->status == DEBUGING
		&& debug(coder, time_start_of_simu) == FALSE)
		return (FALSE);
	else if (coder->status == REFACTORING
		&& refactor(coder, time_start_of_simu) == FALSE)
		return (FALSE);
	return (TRUE);
}

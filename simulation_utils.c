/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:53:53 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/05 23:09:55 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_status	get_next_step(t_status current_step)
{
	if (current_step == COMPILING)
		return (DEBUGING);
	else if (current_step == DEBUGING)
		return (REFACTORING);
	else
		return (COMPILING);
}

void	print_log(char *log, uint64_t time_start_of_simu,
	t_coder *coder, t_bool burnout)
{
	pthread_mutex_lock(coder->log_mutex);
	if (simulation_is_stopped(coder) == TRUE)
	{
		pthread_mutex_unlock(coder->log_mutex);
		return ;
	}
	printf("%lld ", (long long int)(true_get_time_of_day()
			- time_start_of_simu));
	printf("%d ", coder->id);
	printf("%s", log);
	if (burnout == TRUE)
		stop_simulation(coder);
	pthread_mutex_unlock(coder->log_mutex);
}

t_bool	check_dongles_cooldowns(t_coder *coder)
{
	uint64_t	current_time;

	current_time = true_get_time_of_day();
	if (coder->dongle_1->last_usage
		+ coder->dongle_1->dongle_cd <= current_time
		&& coder->dongle_2->last_usage
		+ coder->dongle_2->dongle_cd <= current_time)
		return (TRUE);
	else
		return (FALSE);
}

void	update_dongle_cooldown(t_coder *coder)
{
	coder->dongle_1->last_usage = true_get_time_of_day();
	coder->dongle_2->last_usage = coder->dongle_1->last_usage;
}

t_bool	check_if_coder_can_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->dongle_1->heap.heap_mutex);
	pthread_mutex_lock(&coder->dongle_2->heap.heap_mutex);
	if (coder->dongle_1->heap.requests[0].coder_id != coder->id
		|| coder->dongle_2->heap.requests[0].coder_id != coder->id)
	{
		pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
		pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
		return (FALSE);
	}
	pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
	pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
	if (check_dongles_cooldowns(coder) == FALSE
		|| simulation_is_stopped(coder) == TRUE)
	{
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		return (FALSE);
	}
	else
		return (TRUE);
}

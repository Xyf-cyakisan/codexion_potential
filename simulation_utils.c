/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:53:53 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/06 14:36:35 by cyakisan         ###   ########.fr       */
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

void	update_dongle_cooldown(t_coder *coder)
{
	coder->dongle_1->last_usage = true_get_time_of_day();
	coder->dongle_2->last_usage = coder->dongle_1->last_usage;
}

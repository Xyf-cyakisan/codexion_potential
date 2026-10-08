/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:59:15 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 15:34:25 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_bool	update_last_compile(t_coder *coder)
{
	uint64_t	last_compile;
	uint64_t	current_time;

	pthread_mutex_lock(&coder->compile_mutex);
	if (simulation_is_stopped(coder) == TRUE)
	{
		pthread_mutex_unlock(&coder->compile_mutex);
		return (FALSE);
	}
	last_compile = coder->last_compile;
	if (last_compile == 0)
		last_compile = get_time_start_of_simulation(coder);
	current_time = true_get_time_of_day();
	if (last_compile + coder->time_burnout <= current_time)
	{
		pthread_mutex_unlock(&coder->compile_mutex);
		return (FALSE);
	}
	coder->last_compile = current_time;
	pthread_mutex_unlock(&coder->compile_mutex);
	return (TRUE);
}

static void	print_compile_log(t_coder *coder, uint64_t time_start_of_simu)
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
	printf("%s", "has taken a dongle\n");
	printf("%lld ", (long long int)(true_get_time_of_day()
			- time_start_of_simu));
	printf("%d ", coder->id);
	printf("%s", "has taken a dongle\n");
	printf("%lld ", (long long int)(true_get_time_of_day()
			- time_start_of_simu));
	printf("%d ", coder->id);
	printf("%s", "is compiling\n");
	pthread_mutex_unlock(coder->log_mutex);
}

void	print_burnout_log(char *log, uint64_t time_start_of_simu,
	t_coder *coder)
{
	pthread_mutex_lock(coder->log_mutex);
	printf("%lld ", (long long int)(true_get_time_of_day()
			- time_start_of_simu));
	printf("%d ", coder->id);
	printf("%s", log);
	pthread_mutex_unlock(coder->log_mutex);
}

t_bool	real_compile(t_coder *coder, uint64_t time_start_of_simu)
{
	if (update_last_compile(coder) == FALSE)
	{
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		return (FALSE);
	}
	print_compile_log(coder, time_start_of_simu);
	if (thread_sleep(coder,
			true_get_time_of_day() + coder->time_compile) == FALSE)
	{
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		return (FALSE);
	}
	return (TRUE);
}

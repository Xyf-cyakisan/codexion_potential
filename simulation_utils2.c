/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 22:02:07 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/05 22:02:27 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	simulation_is_stopped(t_coder *coder)
{
	t_bool	stop;

	pthread_mutex_lock(coder->state_mutex);
	stop = *coder->stop;
	pthread_mutex_unlock(coder->state_mutex);
	return (stop);
}

t_bool	simulation_is_started(t_coder *coder)
{
	t_bool	started;

	pthread_mutex_lock(coder->state_mutex);
	started = *coder->simu_started;
	pthread_mutex_unlock(coder->state_mutex);
	return (started);
}

int	get_required_compilations(t_coder *coder)
{
	int	required;

	pthread_mutex_lock(&coder->nb_comp);
	required = coder->required_compilations;
	pthread_mutex_unlock(&coder->nb_comp);
	return (required);
}

uint64_t	get_time_start_of_simulation(t_coder *coder)
{
	uint64_t	time_start;

	pthread_mutex_lock(coder->state_mutex);
	time_start = *coder->time_start_of_simu;
	pthread_mutex_unlock(coder->state_mutex);
	return (time_start);
}

void	stop_simulation(t_coder *coder)
{
	pthread_mutex_lock(coder->state_mutex);
	*coder->stop = TRUE;
	pthread_mutex_unlock(coder->state_mutex);
}

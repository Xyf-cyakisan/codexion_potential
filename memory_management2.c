/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 15:54:54 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 14:56:19 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	clean_threads(t_simulation *simu)
{
	int	i;

	i = 0;
	while (i < simu->nb_coders)
	{
		pthread_join(simu->coders[i].thread, NULL);
		++i;
	}
	pthread_mutex_lock(&simu->state_mutex);
	simu->stop = TRUE;
	pthread_mutex_unlock(&simu->state_mutex);
	pthread_join(simu->monitor.checker_thread, NULL);
}

void	clean_side_mutexes_and_cond(t_simulation *simu)
{
	pthread_cond_destroy(&simu->cond);
	pthread_mutex_destroy(&simu->cond_mutex);
	pthread_mutex_destroy(&simu->state_mutex);
	pthread_mutex_destroy(&simu->log_mutex);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 16:36:00 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 15:53:48 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_simulation(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->nb_coders)
	{
		simulation->coders[i].time_start_of_simu = (&simulation
				->time_start_of_simu);
		++i;
	}
	simulation->time_start_of_simu = true_get_time_of_day();
	pthread_mutex_lock(&simulation->state_mutex);
	simulation->simu_started = TRUE;
	pthread_mutex_unlock(&simulation->state_mutex);
}

void	true_init_mutexes(t_simulation *simu, int *i)
{
	while (*i < simu->nb_coders)
	{
		if (pthread_mutex_init(&simu->dongles[*i].mutex, NULL) != 0)
			break ;
		if (pthread_mutex_init(&simu->dongles[*i].heap.heap_mutex, NULL) != 0)
		{
			pthread_mutex_destroy(&simu->dongles[*i].mutex);
			break ;
		}
		if (pthread_mutex_init(&simu->coders[*i].compile_mutex, NULL) != 0)
		{
			pthread_mutex_destroy(&simu->dongles[*i].heap.heap_mutex);
			pthread_mutex_destroy(&simu->dongles[*i].mutex);
			break ;
		}
		if (pthread_mutex_init(&simu->coders[*i].nb_comp, NULL) != 0)
		{
			pthread_mutex_destroy(&simu->coders[*i].compile_mutex);
			pthread_mutex_destroy(&simu->dongles[*i].heap.heap_mutex);
			pthread_mutex_destroy(&simu->dongles[*i].mutex);
			break ;
		}
		*i += 1;
	}
}

void	destroy_partial_mutexes(t_simulation *simu, int count)
{
	while (count-- > 0)
	{
		pthread_mutex_destroy(&simu->coders[count].nb_comp);
		pthread_mutex_destroy(&simu->coders[count].compile_mutex);
		pthread_mutex_destroy(&simu->dongles[count].heap.heap_mutex);
		pthread_mutex_destroy(&simu->dongles[count].mutex);
	}
}

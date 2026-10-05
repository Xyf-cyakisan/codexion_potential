/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 16:36:00 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/02 16:58:37 by cyakisan         ###   ########.fr       */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 14:37:47 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/06 13:26:59 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	set_other_values(t_simulation *simulation, int nb_coders)
{
	int	i;

	i = 0;
	while (i < nb_coders)
	{
		simulation->coders[i].cond_mutex = &simulation->cond_mutex;
		simulation->coders[i].cond = &simulation->cond;
		++i;
	}
}

void	set_monitor_values(t_simulation *simulation,
			t_monitor *monitor)
{
	int	i;

	i = 0;
	monitor->coders = simulation->coders;
	simulation->stop = FALSE;
	monitor->stop = &simulation->stop;
	*monitor->stop = simulation->stop;
	monitor->log_mutex = &simulation->log_mutex;
	monitor->state_mutex = &simulation->state_mutex;
	simulation->simu_started = FALSE;
	while (i < simulation->nb_coders)
	{
		simulation->coders[i].stop = &simulation->stop;
		simulation->coders[i].simu_started = &simulation->simu_started;
		++i;
	}
	monitor->nb_coders = simulation->nb_coders;
	monitor->simu_started = &simulation->simu_started;
	monitor->cond = &simulation->cond;
	monitor->cond_mutex = &simulation->cond_mutex;
}

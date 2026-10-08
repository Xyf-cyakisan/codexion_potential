/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:41:17 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 14:35:35 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_bool	init_dongle_mutexes(t_simulation *simu)
{
	int	i;

	i = 0;
	while (i < simu->nb_coders)
	{
		if (pthread_mutex_init(&simu->dongles[i].mutex, NULL) != 0
			|| pthread_mutex_init(&simu->dongles[i].heap.heap_mutex, NULL) != 0
			|| pthread_mutex_init(&simu->coders[i].compile_mutex, NULL)
			|| pthread_mutex_init(&simu->coders[i].nb_comp, NULL))
		{
			while (i-- > 0)
			{
				pthread_mutex_destroy(&simu->dongles[i].heap.heap_mutex);
				pthread_mutex_destroy(&simu->dongles[i].mutex);
				pthread_mutex_destroy(&simu->coders[i].compile_mutex);
				pthread_mutex_destroy(&simu->coders[i].nb_comp);
			}
			return (display_error(ERR_MUTEX_INIT, 9, 0), FALSE);
		}
		++i;
	}
	return (TRUE);
}

static void	*run_single_simulation(void *arg)
{
	t_coder		*coder;
	int			required_comps_beg;
	t_request	request;

	coder = arg;
	ft_bzero(&request, sizeof(t_request));
	while (simulation_is_started(coder) == FALSE)
		usleep(1000);
	required_comps_beg = get_required_compilations(coder);
	while (get_required_compilations(coder) != 0
		&& simulation_is_stopped(coder) == FALSE)
	{
		if (coder->nb_coders == 1)
			usleep(1000);
		else
		{
			if (coder_act(coder, get_time_start_of_simulation(coder),
					required_comps_beg, request) == FALSE)
				break ;
			coder->status = get_next_step(coder->status);
		}
	}
	return (NULL);
}

static t_bool	init_threads(t_simulation *simu)
{
	int		i;

	i = 0;
	if (pthread_create(&simu->monitor.checker_thread, NULL,
			monitor, &simu->monitor) != 0)
		return (pthread_join(simu->monitor.checker_thread, NULL),
			display_error(ERR_THREADS_INIT, 10, 0), FALSE);
	while (i < simu->nb_coders)
	{
		if (pthread_create(&simu->coders[i].thread, NULL, run_single_simulation,
				&simu->coders[i]) != 0)
		{
			while (i-- > 0)
				pthread_join(simu->coders[i].thread, NULL);
			return (pthread_join(simu->monitor.checker_thread, NULL),
				display_error(ERR_THREADS_INIT, 10, 0),
				simu->simu_started = TRUE, simu->stop = TRUE, FALSE);
		}
		++i;
	}
	init_simulation(simu);
	return (TRUE);
}

static void	set_heaps_beginning(t_simulation *simulation)
{
	int	parity;
	int	i;

	parity = 0;
	while (parity < 2)
	{
		i = parity;
		while (i < simulation->nb_coders)
		{
			heap_add_back(&simulation->coders[i].dongle_1->heap,
				new_request(&simulation->coders[i]));
			heap_add_back(&simulation->coders[i].dongle_2->heap,
				new_request(&simulation->coders[i]));
			i += 2;
		}
		parity++;
	}
}

t_bool	run_whole_simulation(t_simulation *simulation)
{
	set_heaps_beginning(simulation);
	if (init_dongle_mutexes(simulation) == FALSE
		|| init_threads(simulation) == FALSE)
		return (clean_base_objects(simulation), FALSE);
	return (TRUE);
}

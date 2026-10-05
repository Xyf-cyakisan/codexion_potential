/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:10:34 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/05 22:19:46 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "codexion.h"

static void	set_value(int result, t_config *config, int index)
{
	if (index == 1)
		config->nb_coders = result;
	else if (index == 2)
		config->time_burnout = result;
	else if (index == 3)
		config->time_compile = result;
	else if (index == 4)
		config->time_debug = result;
	else if (index == 5)
		config->time_refactor = result;
	else if (index == 6)
		config->nb_compiles_required = result;
	else if (index == 7)
		config->dongle_cd = result;
}

static t_bool	additional_checks(t_config *config)
{
	if (config->nb_coders == 0)
		return (display_error(ERR_NB_CODERS, 6, 1), FALSE);
	if (config->time_burnout == 0)
		return (display_error(ERR_TIME_BURNOUT, 7, 2), FALSE);
	if (config->nb_compiles_required == 0)
		return (display_error(ERR_REQUIRED_COMP, 8, 6), FALSE);
	return (TRUE);
}

t_bool	parse(int ac, char **av, t_config *config)
{
	int			i;
	int			result;

	if (ac == 9)
	{
		i = 1;
		while (i != ac - 1)
		{
			if (ft_check_number_validity(av[i], i) == FALSE)
				return (FALSE);
			result = ft_atoi(av[i], i);
			if (result == FALSE)
				return (FALSE);
			set_value(result, config, i);
			i++;
		}
		if (ft_check_scheduler(av[i]) == FALSE)
			return (FALSE);
		config->scheduler = av[i];
		if (additional_checks(config) == FALSE)
			return (FALSE);
		return (TRUE);
	}
	else
		return (display_error(ERR_NB_ARGS, 1, 0), FALSE);
}

static void	set_values(t_simulation *simulation,
							t_config config, int nb_dongle)
{
	int	i;

	i = -1;
	while (++i < nb_dongle)
	{
		(simulation->dongles)[i].id = i + 1;
		(simulation->dongles)[i].heap.scheduler = config.scheduler;
		(simulation->dongles)[i].dongle_cd = config.dongle_cd;
	}
	i = -1;
	while (++i < config.nb_coders)
	{
		(simulation->coders)[i].id = i + 1;
		(simulation->coders)[i].status = COMPILING;
		(simulation->coders)[i].required_compilations = (config
				.nb_compiles_required);
		(simulation->coders)[i].time_compile = config.time_compile;
		(simulation->coders)[i].time_burnout = config.time_burnout;
		(simulation->coders)[i].time_debug = config.time_debug;
		(simulation->coders)[i].time_refactor = config.time_refactor;
		(simulation->coders)[i].log_mutex = &simulation->log_mutex;
		(simulation->coders)[i].state_mutex = &simulation->state_mutex;
		set_dongle_one_and_two(simulation, config, i);
		(simulation->coders)[i].nb_coders = config.nb_coders;
	}
}

t_bool	create_objects(t_simulation *simulation, t_config config)
{
	int				nb_dongle;

	simulation->coders = NULL;
	simulation->dongles = NULL;
	if (pthread_mutex_init(&simulation->log_mutex, NULL) != 0)
		return (FALSE);
	if (pthread_mutex_init(&simulation->state_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&simulation->log_mutex), FALSE);
	simulation->nb_coders = config.nb_coders;
	simulation->coders = ft_calloc(config.nb_coders, sizeof(t_coder));
	if (!simulation->coders)
		return (pthread_mutex_destroy(&simulation->state_mutex),
			pthread_mutex_destroy(&simulation->log_mutex), FALSE);
	nb_dongle = config.nb_coders;
	simulation->dongles = ft_calloc(nb_dongle, sizeof(t_dongle));
	if (!simulation->dongles)
		return (clean_base_objects(simulation),
			pthread_mutex_destroy(&simulation->state_mutex),
			pthread_mutex_destroy(&simulation->log_mutex), FALSE);
	set_values(simulation, config, nb_dongle);
	set_monitor_values(simulation, &simulation->monitor);
	return (TRUE);
}

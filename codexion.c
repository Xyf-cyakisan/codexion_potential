/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:33:55 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/01 16:06:42 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "codexion.h"

int	main(int ac, char **av)
{
	t_config			config;
	t_simulation		simulation;

	if (parse(ac, av, &config) == FALSE)
		return (1);
	if (create_objects(&simulation, config) == FALSE)
		return (1);
	if (run_whole_simulation(&simulation) == FALSE)
		return (1);
	clean_threads(&simulation);
	clean_mutexes(&simulation);
	clean_base_objects(&simulation);
	return (0);
}

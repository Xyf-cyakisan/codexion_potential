/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 14:12:48 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/06 13:27:10 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "general_utils.h"
# include "structures.h"
# include "memory_management.h"

t_bool	ft_check_number_validity(char *str, int arg_index);
t_bool	ft_check_scheduler(char *str);
int		ft_atoi(const char *nptr, int arg_index);
t_bool	parse(int ac, char **av, t_config *config);
void	set_dongle_one_and_two(t_simulation *simulation,
			t_config config, int i);
void	set_other_values(t_simulation *simulation, int nb_coders);
void	set_monitor_values(t_simulation *simulation,
			t_monitor *monitor);
t_bool	create_objects(t_simulation *simulation, t_config config);

#endif
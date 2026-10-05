/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:26:54 by cyakisan          #+#    #+#             */
/*   Updated: 2026/09/25 17:36:14 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general_utils.h"

t_bool	ft_check_number_validity(char *str, int arg_index)
{
	size_t	i;

	i = 0;
	if (!str)
		return (display_error(ERR_EMPTY_STR, 2, 0), FALSE);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (display_error(ERR_POS_INTS,
					3, arg_index), FALSE);
		++i;
	}
	return (TRUE);
}

t_bool	ft_check_scheduler(char *str)
{
	if (strcmp(str, "fifo") && strcmp(str, "edf"))
		return (display_error(ERR_SCHEDULER, 4, 9), FALSE);
	return (TRUE);
}

int	ft_atoi(const char *nptr, int arg_index)
{
	unsigned int	i;
	long int		result;

	i = 0;
	result = 0;
	if (ft_strlen(nptr) >= 11)
		return (display_error(ERR_BIGGER_INT_MAX, 5, arg_index), FALSE);
	while (nptr[i] != '\0')
	{
		result = result * 10 + (nptr[i] - '0');
		++i;
	}
	if (result > INT_MAX)
		return (display_error(ERR_BIGGER_INT_MAX, 5, arg_index), FALSE);
	return ((int)result);
}

void	set_dongle_one_and_two(t_simulation *simulation, t_config config, int i)
{
	t_dongle	*swapper;

	(simulation->coders)[i].dongle_1 = &((simulation->dongles)[i]);
	(simulation->coders)[i].dongle_2 = &((simulation->dongles)[
			(i + 1) % config.nb_coders]);
	if ((simulation->coders)[i].dongle_1->id
	> (simulation->coders)[i].dongle_2->id)
	{
		swapper = (simulation->coders)[i].dongle_1;
		(simulation->coders)[i].dongle_1 = (simulation->coders)[i].dongle_2;
		(simulation->coders)[i].dongle_2 = swapper;
	}
}

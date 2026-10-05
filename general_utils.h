/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 11:45:27 by cyakisan          #+#    #+#             */
/*   Updated: 2026/09/28 13:48:27 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERAL_UTILS_H
# define GENERAL_UTILS_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "structures.h"
# include <sys/time.h>

size_t		ft_strlen(const char *str);
void		display_error(char *error_msg, int error_id, int arg_index);
uint64_t	true_get_time_of_day(void);

#endif

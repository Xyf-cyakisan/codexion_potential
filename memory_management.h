/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:16:49 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 14:51:47 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_MANAGEMENT_H
# define MEMORY_MANAGEMENT_H

# include "general_utils.h"

void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	clean_base_objects(t_simulation *simulation);
void	clean_mutexes(t_simulation *simu);
void	clean_threads(t_simulation *simu);
void	clean_side_mutexes_and_cond(t_simulation *simu);

#endif
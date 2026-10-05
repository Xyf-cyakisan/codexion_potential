/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 23:47:05 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/05 23:52:13 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	check_if_first(t_coder *coder)
{
	pthread_mutex_lock(&coder->dongle_1->heap.heap_mutex);
	pthread_mutex_lock(&coder->dongle_2->heap.heap_mutex);
	if (coder->dongle_1->heap.requests[0].coder_id != coder->id
		|| coder->dongle_2->heap.requests[0].coder_id != coder->id)
	{
		pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
		pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
		return (FALSE);
	}
	pthread_mutex_unlock(&coder->dongle_2->heap.heap_mutex);
	pthread_mutex_unlock(&coder->dongle_1->heap.heap_mutex);
	return (TRUE);
}

void	update_required_compilations(t_coder *coder)
{
	pthread_mutex_lock(&coder->nb_comp);
	coder->required_compilations--;
	pthread_mutex_unlock(&coder->nb_comp);
}

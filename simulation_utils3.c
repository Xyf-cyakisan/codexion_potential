/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 23:47:05 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 17:01:12 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_bool	check_if_first(t_coder *coder)
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

void	cond_broadcast(pthread_cond_t *cond, pthread_mutex_t *cond_mutex)
{
	pthread_mutex_lock(cond_mutex);
	pthread_cond_broadcast(cond);
	pthread_mutex_unlock(cond_mutex);
}

static void	wait_for_dongle_cd(t_coder *coder)
{
	uint64_t	current_time;
	t_dongle	*dongle;

	current_time = true_get_time_of_day();
	if (coder->dongle_1->dongle_cd + coder->dongle_1->last_usage <= current_time
		&& coder->dongle_2->dongle_cd
		+ coder->dongle_2->last_usage <= current_time)
		return ;
	if (coder->dongle_1->dongle_cd + coder->dongle_1->last_usage
		> coder->dongle_2->dongle_cd
		+ coder->dongle_2->last_usage)
		dongle = coder->dongle_1;
	else
		dongle = coder->dongle_2;
	if (thread_sleep(coder, (dongle->dongle_cd
				- (current_time - dongle->last_usage) + current_time)) == FALSE)
		return ;
}

t_bool	wait_for_cond(t_coder *coder)
{
	pthread_mutex_lock(coder->cond_mutex);
	while (check_if_first(coder) == FALSE
		&& simulation_is_stopped(coder) == FALSE)
		pthread_cond_wait(coder->cond, coder->cond_mutex);
	pthread_mutex_unlock(coder->cond_mutex);
	if (simulation_is_stopped(coder) == TRUE)
		return (FALSE);
	wait_for_dongle_cd(coder);
	return (TRUE);
}

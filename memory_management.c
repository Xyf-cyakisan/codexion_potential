/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:12:16 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 13:18:58 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general_utils.h"
#include "memory_management.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned int	i;
	unsigned char	*temp;

	i = 0;
	temp = s;
	while (i < n)
	{
		temp[i] = '\0';
		++i;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*tofill;

	total_size = nmemb * size;
	if (size && nmemb && nmemb > (size_t)-1 / size)
		return (display_error("Memory allocation failed", 9, 0), NULL);
	tofill = malloc(total_size);
	if (tofill == NULL)
		return (display_error("Memory allocation failed", 9, 0), NULL);
	ft_bzero(tofill, total_size);
	return (tofill);
}

void	clean_base_objects(t_simulation *simulation)
{
	if (simulation->coders)
		free(simulation->coders);
	if (simulation->dongles)
		free(simulation->dongles);
	simulation->coders = NULL;
	simulation->dongles = NULL;
}

void	clean_mutexes(t_simulation *simu)
{
	int	i;

	i = 0;
	while (i < simu->nb_coders)
	{
		pthread_mutex_destroy(&simu->coders[i].compile_mutex);
		pthread_mutex_destroy(&simu->dongles[i].mutex);
		pthread_mutex_destroy(&simu->dongles[i].heap.heap_mutex);
		pthread_mutex_destroy(&simu->coders[i].nb_comp);
		++i;
	}
	pthread_cond_destroy(&simu->cond);
	pthread_mutex_destroy(&simu->cond_mutex);
	pthread_mutex_destroy(&simu->state_mutex);
	pthread_mutex_destroy(&simu->log_mutex);
}

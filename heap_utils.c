/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:52:12 by cyakisan          #+#    #+#             */
/*   Updated: 2026/09/28 16:04:26 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"
#include "general_utils.h"

t_request	new_request(t_coder *coder)
{
	t_request	request;

	request.coder_id = coder->id;
	request.last_compile = coder->last_compile;
	return (request);
}

void	heap_add_back(t_heap *heap, t_request request)
{
	if (heap->size == 2)
		return ;
	if (!strcmp(heap->scheduler, "edf")
		&& heap->size == 1
		&& heap->requests[0].last_compile > request.last_compile)
	{
		heap->requests[1] = heap->requests[0];
		heap->requests[0] = request;
	}
	else
		heap->requests[heap->size] = request;
	heap->size++;
}

void	heap_pop(t_heap *heap)
{
	if (heap->size == 0)
		return ;
	if (heap->size == 2)
		heap->requests[0] = heap->requests[1];
	--heap->size;
}

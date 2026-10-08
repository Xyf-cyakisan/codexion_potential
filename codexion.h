/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:14:03 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/08 13:28:09 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "general_utils.h"
# include <pthread.h>
# include <stdint.h>
# include "structures.h"
# include <unistd.h>
# include "memory_management.h"

t_request	new_request(t_coder *coder);
void		heap_add_back(t_heap *heap, t_request request);
void		heap_pop(t_heap *heap);
t_status	get_next_step(t_status current_step);
void		print_log(char *log, uint64_t time_start_of_simu,
				t_coder *coder);
void		print_burnout_log(char *log, uint64_t time_start_of_simu,
				t_coder *coder);
void		update_dongle_cooldown(t_coder *coder);
void		update_required_compilations(t_coder *coder);
t_bool		simulation_is_stopped(t_coder *coder);
t_bool		simulation_is_started(t_coder *coder);
void		cond_broadcast(pthread_cond_t *cond, pthread_mutex_t *cond_mutex);
int			get_required_compilations(t_coder *coder);
uint64_t	get_time_start_of_simulation(t_coder *coder);
void		stop_simulation(t_coder *coder);
void		wait_for_dongle_cd(t_coder *coder);
t_bool		wait_for_cond(t_coder *coder);
t_bool		thread_sleep(t_coder *coder, uint64_t end_time);
t_bool		real_compile(t_coder *coder, uint64_t time_start_of_simu);
t_bool		coder_act(t_coder *coder, uint64_t time_start_of_simu,
				int required_comps_beg, t_request request);
void		*monitor(void *arg);
void		init_simulation(t_simulation *simulation);
t_bool		run_whole_simulation(t_simulation *simulation);

#endif

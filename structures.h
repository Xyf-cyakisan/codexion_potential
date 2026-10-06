/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyakisan <cyakisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:07:05 by cyakisan          #+#    #+#             */
/*   Updated: 2026/10/06 13:24:43 by cyakisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <pthread.h>
# include <stdint.h>

# define INT_MAX 2147483647
# define ERR_NB_ARGS "Not the correct amount of arguments (needs 8)"
# define ERR_EMPTY_STR "Empty string encountered"
# define ERR_POS_INTS "First eight arguments must be positive integers"
# define ERR_SCHEDULER "Scheduler must be 'fifo' or 'edf'"
# define ERR_BIGGER_INT_MAX "No number should be bigger than the INT_MAX"
# define ERR_NB_CODERS "Number of coders must be greater than 0"
# define ERR_TIME_BURNOUT "Time to burnout must be greater than 0"
# define ERR_REQUIRED_COMP "Number of compiles required must be greater than 0"
# define ERR_MUTEX_INIT "Mutexes initialization failed"
# define ERR_THREADS_INIT "Threads initialization failed"
# define ERR_EMPTY_NODE_HEAP "NULL node encountered"
# define TRUE 1
# define FALSE -1

typedef int	t_bool;

typedef enum e_status
{
	COMPILING = 1,
	DEBUGING = 2,
	REFACTORING = 3
}	t_status;

typedef struct s_config
{
	const char	*scheduler;
	int			nb_coders;
	int			time_burnout;
	int			time_compile;
	int			time_debug;
	int			time_refactor;
	int			nb_compiles_required;
	int			dongle_cd;
}	t_config;

typedef struct s_request
{
	int				coder_id;
	uint64_t		last_compile;
}	t_request;

typedef struct s_heap
{
	t_request		requests[2];
	size_t			size;
	const char		*scheduler;
	pthread_mutex_t	heap_mutex;
}	t_heap;

typedef struct s_dongle
{
	uint64_t		last_usage;
	int				id;
	t_heap			heap;
	pthread_mutex_t	mutex;
	int				dongle_cd;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	t_status		status;
	uint64_t		last_compile;
	int				required_compilations;
	uint64_t		time_compile;
	uint64_t		time_burnout;
	uint64_t		time_debug;
	uint64_t		time_refactor;
	t_dongle		*dongle_1;
	t_dongle		*dongle_2;
	pthread_t		thread;
	pthread_mutex_t	*log_mutex;
	pthread_mutex_t	*state_mutex;
	uint64_t		*time_start_of_simu;
	t_bool			*simu_started;
	t_bool			*stop;
	pthread_mutex_t	compile_mutex;
	pthread_mutex_t	nb_comp;
	pthread_cond_t	*cond;
	pthread_mutex_t	*cond_mutex;
	int				nb_coders;
}	t_coder;

typedef struct s_monitor
{
	t_coder			*coders;
	pthread_t		checker_thread;
	t_bool			*stop;
	pthread_mutex_t	*log_mutex;
	pthread_mutex_t	*state_mutex;
	int				nb_coders;
	t_bool			*simu_started;
	pthread_cond_t	*cond;
	pthread_mutex_t	*cond_mutex;
}	t_monitor;

typedef struct s_simulation
{
	t_coder			*coders;
	t_dongle		*dongles;
	int				nb_coders;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	state_mutex;
	pthread_cond_t	cond;
	pthread_mutex_t	cond_mutex;
	t_monitor		monitor;
	t_bool			stop;
	t_bool			simu_started;
	uint64_t		time_start_of_simu;
}	t_simulation;

#endif
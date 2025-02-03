/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/02 17:40:37 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>
# include <limits.h>

# define DEBUG_MODE 0

typedef struct s_table	t_table;

typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}	t_fork;

typedef struct s_philo
{
	int			id;
	long		meals_counter;
	bool		is_full;
	long		last_meal_time;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_mtx		philo_mutex;
	t_table		*table;
}	t_philo;

struct s_table
{
	long		nbr_philo;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		nbr_limit_meals;
	long		start_time;
	bool		is_finished;
	bool		are_threads_ready;
	long		threads_running_nbr;
	pthread_t	monitor;
	t_mtx		table_mutex;
	t_mtx		write_mutex;
	t_fork		*forks;
	t_philo		*philos;
};

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_opcode;

typedef enum e_time_code
{
	SECONDS,
	MILLISECONDS,
	MICROSECONDS
}	t_time_code;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED
}	t_philo_status;

long	ft_atoi(const char *str);
int		ft_printf(const char *format, ...);
void	parse_input(t_table *table, char **argv);
void	init_data(t_table *table);
void	error_exit(const char *error);
long	get_time(t_time_code time_code);
void	*safe_malloc(size_t bytes);
void	safe_thread_handle(pthread_t *thread, void *(*f)(void *), void *data,
			t_opcode opcode);
void	safe_mutex_handle(t_mtx *mutex, t_opcode opcode);
void	init_data(t_table *table);
void	dinner_start(t_table *table);
void	wait_all_threads(t_table *table);
void	precise_usleep(long usec, t_table *table);
void	set_bool(t_mtx *mutex, bool *dest, bool value);
bool	get_bool(t_mtx *mutex, bool *value);
void	set_long(t_mtx *mutex, long *dest, long value);
long	get_long(t_mtx *mutex, long *value);
bool	simulation_finished(t_table *table);
void	write_status(t_philo_status status, t_philo *philo, bool debug);
void	*monitor_dinner(void *data);
void	increase_long(t_mtx *mutex, long *value);
bool	all_threads_are_running(t_mtx *mutex, long *threads, long philo_nbr);
void	clean(t_table *table);

#endif

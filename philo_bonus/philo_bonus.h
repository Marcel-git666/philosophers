/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/19 20:48:12 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>

# define DEBUG_MODE 0
# define FORKS_SEM "/forks_sem"
# define WRITE_SEM "/write_sem"
# define START_SEM "/start_sem"
# define DEATH_SEM "/death_sem"
# define ALL_FULL_SEM "/all_full_sem"

typedef struct s_table t_table;


typedef struct s_philo
{
	int		id;
	long	meals_counter;
	long	last_meal_time;
	pid_t	process_id;
	t_table	*table;
}		t_philo;

struct s_table
{
	long	nbr_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	nbr_limit_meals;
	long	start_time;
	sem_t	*death_sem;
	sem_t	*all_full_sem;
	pid_t	monitor_process_id;
	sem_t	*write_sem;
	sem_t	*forks;
	sem_t	*start_sem;
	t_philo	*philos;
};

typedef enum e_sem_opcode
{
	SEM_OPEN,
	SEM_WAIT,
	SEM_POST,
	SEM_CLOSE,
	SEM_UNLINK
}		t_sem_opcode;

typedef enum e_opcode
{
	CREATE,
	KILL,
	WAIT,
	JOIN,
	DETACH
}		t_opcode;

typedef enum e_time_code
{
	SECONDS,
	MILLISECONDS,
	MICROSECONDS
}		t_time_code;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED
}		t_philo_status;

long	ft_atoi(const char *str);
int		ft_printf(const char *format, ...);
void	parse_input(t_table *table, char **argv);
void	init_data(t_table *table);
void	error_exit(const char *error);
long	get_time(t_time_code time_code);
void	*safe_malloc(size_t bytes);
sem_t	*safe_semaphore_handle(const char *name, int initial_value,
			t_sem_opcode opcode, sem_t *sem);
void	safe_process_handle(pid_t *process_id, void *data, void (*f)(void *),
			t_opcode opcode);
void	safe_thread_handle(pthread_t *thread, void *(*f)(void *), void *data,
	t_opcode opcode);
void	dinner_start(t_table *table);
void	precise_usleep(long usec, t_table *table);

bool	simulation_finished(t_table *table);
void	write_status(t_philo_status status, t_philo *philo, bool debug);
void	monitor_dinner(void *data);
void	*monitor_thread(void *data);
void	clean(t_table *table);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/12 16:27:42 by mmravec          ###   ########.fr       */
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

# define DEBUG_MODE 0

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
	bool	is_finished;
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
	WAIT
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
void	init_data(t_table *table);
void	dinner_start(t_table *table);

void	precise_usleep(long usec, t_table *table);

bool	simulation_finished(t_table *table);
void	write_status(t_philo_status status, t_philo *philo, bool debug);
void	monitor_dinner(void *data);
void	clean(t_table *table);

#endif

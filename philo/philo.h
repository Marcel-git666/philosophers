/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2024/11/18 07:59:42 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <errno.h>

typedef struct s_table	t_table;

typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}		t_fork;

typedef struct s_philo
{
	int			id;
	int			meals_counter;
	int			is_full;
	long		last_meal_time;
	t_fork		*left_fork;
	t_fork		*right_fork;
	pthread_t	thread_id;
	t_table		*table;
}		t_philo;

struct s_table
{
	int		nbr_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		nbr_limit_meals;
	long	start_time;
	int		is_finished;
	t_fork	*forks;
	t_philo	*philos;
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
}		t_opcode;

int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
void	parse_input(t_table *table, char **argv);
void	init_data(t_table *table);
void	error_exit(const char *error);
void	safe_thread_handle(pthread_t *thread, void *(*f)(void *), void *data,
			t_opcode opcode);
void	safe_mutex_handle(t_mtx *mutex, t_opcode opcode);

#endif

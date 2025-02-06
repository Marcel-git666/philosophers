/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:23:55 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/06 08:55:00 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	monitor_dinner(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	i = -1;
	sem_wait(table->death_sem);
	safe_semaphore_handle(NULL, 0, SEM_WAIT, table->write_sem);
	ft_printf("[MONITOR] A philosopher has died. Terminating program.\n");
	safe_semaphore_handle(NULL, 0, SEM_POST, table->write_sem);
	while (++i < table->nbr_philo)
	{
		kill(table->philos[i].process_id, SIGKILL);
	}
	exit(0);
}

static void	check_death(t_philo *philo, long current_time)
{
	if (current_time - philo->last_meal_time > philo->table->time_to_die)
	{
		safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->write_sem);
		write_status(DIED, philo, DEBUG_MODE);
		safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->death_sem);
		pthread_exit(NULL);
	}
}

static void	check_full(t_philo *philo, long current_time)
{
	if (philo->table->nbr_limit_meals > 0
		&& philo->meals_counter >= philo->table->nbr_limit_meals)
	{
		ft_printf("[MONITOR THREAD] %l ms: Philosopher %d is full.\n",
			current_time - philo->table->start_time, philo->id);
		safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->write_sem);
		safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->all_full_sem);
		pthread_exit(NULL);
	}
}

void	*monitor_thread(void *data)
{
	t_philo	*philo;
	long	current_time;

	philo = (t_philo *)data;
	while (true)
	{
		safe_semaphore_handle(NULL, 0, SEM_WAIT, philo->table->write_sem);
		current_time = get_time(MILLISECONDS);
		check_death(philo, current_time);
		check_full(philo, current_time);
		safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->write_sem);
		usleep(500);
	}
	return (NULL);
}

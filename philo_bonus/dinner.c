/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:23:27 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/05 22:14:16 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	eat(t_philo *philo)
{
	// Wait for two forks (semaphores)
	safe_semaphore_handle(FORKS_SEM, 0, SEM_WAIT, philo->table->forks);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_WAIT, philo->table->forks);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);

	// Update meal count and last meal time
	safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, philo->table->write_sem);
	philo->last_meal_time = get_time(MILLISECONDS);
	philo->meals_counter++;
	safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, philo->table->write_sem);

	write_status(EATING, philo, DEBUG_MODE);
	usleep(philo->table->time_to_eat * 1000);

	// Release the forks (semaphores)
	safe_semaphore_handle(FORKS_SEM, 0, SEM_POST, philo->table->forks);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_POST, philo->table->forks);
}


void	dinner_simulation(void *data)
{
	t_philo		*philo;
	pthread_t	monitor_tid;

	philo = (t_philo *)data;
	safe_thread_handle(&monitor_tid, monitor_thread, philo, CREATE);
	safe_semaphore_handle(NULL, 0, SEM_WAIT, philo->table->start_sem);
	safe_semaphore_handle(NULL, 0, SEM_WAIT, philo->table->write_sem);
	philo->last_meal_time = get_time(MILLISECONDS);
	safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->write_sem);

	while (true)
	{
		eat(philo);
		safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, philo->table->write_sem);
		if (philo->table->nbr_limit_meals > 0 && philo->meals_counter >= philo->table->nbr_limit_meals)
		{
			safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, philo->table->write_sem);
			break;
		}
		safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, philo->table->write_sem);
		write_status(SLEEPING, philo, DEBUG_MODE);
		usleep(philo->table->time_to_sleep * 1000);
		write_status(THINKING, philo, DEBUG_MODE);
	}
	safe_semaphore_handle(FORKS_SEM, 0, SEM_POST, philo->table->forks);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_POST, philo->table->forks);
	safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, philo->table->write_sem);
	ft_printf("[DEBUG] Join monitor thread.\n");
	safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, philo->table->write_sem);
	safe_thread_handle(&monitor_tid, NULL, NULL, JOIN);
	exit (0);
}

void	dinner_start(t_table *table)
{
	int		i;

	table->start_time = get_time(MILLISECONDS);
	i = -1;
	while (++i < table->nbr_philo)
	{
		safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, table->write_sem);
		ft_printf("Creating philosopher process %d\n", i + 1);
		safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, table->write_sem);
		safe_process_handle(&table->philos[i].process_id, table->philos + i,
			dinner_simulation, CREATE);
	}
	safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, table->write_sem);
	ft_printf("[DEBUG] Creating monitor process\n");
	safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, table->write_sem);
	safe_process_handle(&table->monitor_process_id, (void *)table,
		monitor_dinner, CREATE);
	i = -1;
	while (++i < table->nbr_philo)
		safe_semaphore_handle(NULL, 0, SEM_POST, table->start_sem);
	i = -1;
	while (++i < table->nbr_philo)
		safe_process_handle(&table->philos[i].process_id, NULL, NULL, WAIT);
	safe_process_handle(&table->monitor_process_id, NULL, NULL, KILL);
}

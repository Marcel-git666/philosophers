/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:23:27 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/17 16:19:27 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	think(t_philo *philo)
{
	write_status(THINKING, philo, DEBUG_MODE);
}

static void	*lone_philo(t_table *table)
{
	pid_t	process_id;

	process_id = fork();
	if (process_id == 0)
	{
		write_status(TAKE_FIRST_FORK, &table->philos[0], DEBUG_MODE);
		usleep(table->time_to_die * 1000);
		write_status(DIED, &table->philos[0], DEBUG_MODE);
		exit(0);
	}
	else if (process_id < 0)
	{
		error_exit("Fork failed for single philosopher.");
	}
	waitpid(process_id, NULL, 0);
	return (NULL);
}

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
	if (philo->id % 2 == 0)
		usleep(philo->table->time_to_eat * 1000);
	while (true)
	{
		eat(philo);
		if (philo->table->nbr_limit_meals > 0 && philo->meals_counter >= philo->table->nbr_limit_meals)
			break;
		write_status(SLEEPING, philo, DEBUG_MODE);
		usleep(philo->table->time_to_sleep * 1000);
		think(philo);
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
	if (table->nbr_philo == 1)
	{
		lone_philo(table);
		return ;
	}
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
	{
		// safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, table->write_sem);
		// ft_printf("[DEBUG] Posting to start semaphore for philosopher %d\n", i + 1);
		// safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, table->write_sem);
		safe_semaphore_handle(NULL, 0, SEM_POST, table->start_sem);
	}


	// Wait for all philosopher processes to finish
	i = -1;
	while (++i < table->nbr_philo)
		safe_process_handle(&table->philos[i].process_id, NULL, NULL, WAIT);

	// Kill the monitor process once all philosophers are done

	safe_process_handle(&table->monitor_process_id, NULL, NULL, KILL);
}

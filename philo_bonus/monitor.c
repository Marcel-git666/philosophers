/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:23:55 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/16 17:41:15 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// static bool	philo_died(t_philo *philo)
// {
// 	long	last_meal_time;
// 	long	current_time;
// 	long	t_t_die;

// 	current_time = get_time(MILLISECONDS);
// 	safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, philo->table->write_sem);
// 	last_meal_time = philo->last_meal_time;
// 	t_t_die = philo->table->time_to_die;
// 	safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, philo->table->write_sem);
// 	if (current_time - last_meal_time > t_t_die)
// 	{
// 		safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, philo->table->write_sem);
// 		ft_printf("[MONITOR] Philosopher %d died. Current time: %l, Last meal: %l, Time to die: %l\n",
// 			philo->id, current_time, last_meal_time, t_t_die);
// 		safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, philo->table->write_sem);
// 		return (true);
// 	}

// 	return (false);
// }

void	monitor_dinner(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	i = -1;
	safe_semaphore_handle(NULL, 0, SEM_WAIT, table->finished_sem);
	while (++i < table->nbr_philo)
		kill(table->philos[i].process_id, SIGKILL);
	exit(0);
}


void	*monitor_thread(void *data)
{
	t_philo	*philo;
	long	current_time;

	philo = (t_philo *)data;
	while (true)
	{
		current_time = get_time(MILLISECONDS);
		if (current_time - philo->last_meal_time > philo->table->time_to_die)
		{
			write_status(DIED, philo, DEBUG_MODE);
			safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->finished_sem);
			exit(1); // Terminate process on death
		}

		if (philo->table->nbr_limit_meals > 0 && philo->meals_counter >= philo->table->nbr_limit_meals)
		{
			safe_semaphore_handle(NULL, 0, SEM_WAIT, philo->table->write_sem);
			ft_printf("[MONITOR THREAD] Philosopher %d is full.\n", philo->id);
			safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->write_sem);
			exit(0); // Terminate process when full
		}
		usleep(1000);
	}
	return (NULL);
}

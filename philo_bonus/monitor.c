/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:23:55 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/12 16:10:40 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	philo_died(t_philo *philo)
{
	long	last_meal_time;
	long	current_time;
	long	t_t_die;

	current_time = get_time(MILLISECONDS);
	safe_semaphore_handle(NULL, 0, SEM_WAIT, philo->table->write_sem);
	last_meal_time = philo->last_meal_time;
	t_t_die = philo->table->time_to_die;
	safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->write_sem);
	if (current_time - last_meal_time > t_t_die)
		return (true);
	return (false);
}

void	monitor_dinner(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	while (true)
	{
		i = -1;
		while (++i < table->nbr_philo)
		{
			if (philo_died(table->philos + i))
			{
				// Mark simulation as finished and print death status
				safe_semaphore_handle(NULL, 0, SEM_WAIT, table->write_sem);
				table->is_finished = true;
				write_status(DIED, table->philos + i, DEBUG_MODE);
				safe_semaphore_handle(NULL, 0, SEM_POST, table->write_sem);

				// Kill all philosopher processes
				for (int j = 0; j < table->nbr_philo; j++)
					safe_process_handle(&table->philos[j].process_id, NULL, NULL, KILL);

				exit(0);
			}
		}

		// If a meal limit is set, check if all philosophers are full
		if (table->nbr_limit_meals > 0)
		{
			int	full_philos = 0;

			for (i = 0; i < table->nbr_philo; i++)
			{
				safe_semaphore_handle(NULL, 0, SEM_WAIT, table->write_sem);
				if (table->philos[i].meals_counter >= table->nbr_limit_meals)
					full_philos++;
				safe_semaphore_handle(NULL, 0, SEM_POST, table->write_sem);
			}

			if (full_philos == table->nbr_philo)
				exit(0); // All philosophers have eaten enough
		}

		usleep(1000); // Avoid excessive CPU usage
	}
}

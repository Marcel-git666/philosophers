/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:23:55 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/05 19:46:15 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	monitor_dinner(void *data)
{
	t_table	*table;
	int		i;
	long	current_time;

	table = (t_table *)data;
	sem_wait(table->death_sem); 
	while (true)
	{
		usleep(1000);
		// ✅ Check for death
		i = -1;
		while (++i < table->nbr_philo)
		{
			safe_semaphore_handle(NULL, 0, SEM_WAIT, table->philos[i].last_meal_sem);
			current_time = get_time(MILLISECONDS);
			if (current_time - table->philos[i].last_meal_time >= table->time_to_die)
			{
				safe_semaphore_handle(NULL, 0, SEM_POST, table->philos[i].last_meal_sem);
				write_status(DIED, &table->philos[i], DEBUG_MODE);
				safe_semaphore_handle(NULL, 0, SEM_POST, table->death_sem);
				return;
			}
			safe_semaphore_handle(NULL, 0, SEM_POST, table->philos[i].last_meal_sem);
		}

		// ✅ Check if all philosophers have eaten required meals
		i = -1;
		while (++i < table->nbr_philo)
		{
			safe_semaphore_handle(NULL, 0, SEM_WAIT, table->philos[i].last_meal_sem);
			if (table->nbr_limit_meals > 0 && table->philos[i].meals_counter < table->nbr_limit_meals)
			{
				safe_semaphore_handle(NULL, 0, SEM_POST, table->philos[i].last_meal_sem);
				break;
			}
			safe_semaphore_handle(NULL, 0, SEM_POST, table->philos[i].last_meal_sem);
		}
		if (i == table->nbr_philo) // ✅ If all philosophers have eaten enough
		{
			safe_semaphore_handle(NULL, 0, SEM_POST, table->all_full_sem);
			exit(0);
		}
		safe_semaphore_handle(NULL, 0, SEM_POST, table->death_sem);
		exit(0);
	}
}

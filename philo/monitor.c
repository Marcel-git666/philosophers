/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:23:55 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/29 19:48:10 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo)
{
	long	last_meal_time;
	long	current_time;
	long	t_t_die;

	if (get_bool(&philo->philo_mutex, &philo->is_full))
		return (false);
	current_time = get_time(MILLISECONDS);
	if (!get_long(&philo->philo_mutex, &philo->last_meal_time))
		return (false);
	last_meal_time = philo->last_meal_time;
	t_t_die = philo->table->time_to_die;
	if (last_meal_time == philo->table->start_time)
		return (false);
	if (current_time - last_meal_time > t_t_die)
		return (true);
	return (false);
}

void	*monitor_dinner(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	while (!all_threads_are_running(&table->table_mutex,
			&table->threads_running_nbr, table->nbr_philo))
		;
	while (!simulation_finished(table))
	{
		i = -1;
		while (++i < table->nbr_philo && !simulation_finished(table))
		{
			if (philo_died(table->philos + i))
			{
				if (!set_bool(&table->table_mutex, &table->is_finished, true))
					return (NULL);
				write_status(DIED, table->philos + i, DEBUG_MODE);
				return (NULL);
			}
		}
		usleep(500);
	}
	return (NULL);
}

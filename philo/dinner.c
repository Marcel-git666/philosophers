/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:23:27 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/29 16:38:35 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*lone_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *) data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECONDS));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nbr);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!simulation_finished(philo->table))
	{
		usleep(200);
	}
	return (NULL);
}

static void	eat(t_philo *philo)
{
	safe_mutex_handle(&philo->first_fork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	safe_mutex_handle(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	safe_mutex_handle(&philo->philo_mutex, LOCK);
	philo->meals_counter++;
	safe_mutex_handle(&philo->philo_mutex, UNLOCK);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECONDS));
	write_status(EATING, philo, DEBUG_MODE);
	safe_sleep(philo, philo->table->time_to_eat);
	if (philo->table->nbr_limit_meals > 0
		&& philo->meals_counter == philo->table->nbr_limit_meals)
		set_bool(&philo->philo_mutex, &philo->is_full, true);
	safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
	safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *) data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECONDS));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nbr);
	if (philo->id % 2 == 0)
		usleep(100);
	while (!simulation_finished(philo->table))
	{
		// 1) am I full?
		if (philo->is_full)
			break;
		if (philo->is_dead)
		{
			write_status(DIED, philo, DEBUG_MODE);
			break;
		}
		// 2) eat
		eat(philo);
		if (philo->is_dead)
		{
			write_status(DIED, philo, DEBUG_MODE);
			break;
		}
		// 3) sleep -> write status
		write_status(SLEEPING, philo, DEBUG_MODE);
		safe_sleep(philo, philo->table->time_to_sleep);
		if (philo->is_dead)
		{
			write_status(DIED, philo, DEBUG_MODE);
			break;
		}
		// 4) think
		write_status(THINKING, philo, DEBUG_MODE);
	}
	return (NULL);
}

void	dinner_start(t_table *table)
{
	int		i;

	i = -1;
	if (table->nbr_limit_meals == 0)
		return ;
	else if (table->nbr_philo == 1)
	{
		safe_thread_handle(&table->philos[0].thread_id, lone_philo,
			&table->philos[0], CREATE);
	}
	else
	{
		while (++i < table->nbr_philo)
			safe_thread_handle(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
	}
	safe_thread_handle(&table->monitor, monitor_dinner, table, CREATE);
	safe_mutex_handle(&table->table_mutex, LOCK);
	table->start_time = get_time(MILLISECONDS);
	safe_mutex_handle(&table->table_mutex, UNLOCK);
	set_bool(&table->table_mutex, &table->are_threads_ready, true);
	i = -1;
	while (++i < table->nbr_philo)
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->is_finished, true);
	safe_thread_handle(&table->monitor, NULL, NULL, JOIN);
}

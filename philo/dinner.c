/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:23:27 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/29 19:41:30 by mmravec          ###   ########.fr       */
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
	if (!safe_mutex_handle(&philo->first_fork->fork, LOCK))
		return ;
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	if (!safe_mutex_handle(&philo->second_fork->fork, LOCK))
	{
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		return ;
	}
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	if (!safe_mutex_handle(&philo->philo_mutex, LOCK))
	{
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
		return ;
	}
	philo->meals_counter++;
	safe_mutex_handle(&philo->philo_mutex, UNLOCK);
	if (!safe_mutex_handle(&philo->philo_mutex, LOCK))
	{
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
		return ;
	}
	philo->last_meal_time = get_time(MILLISECONDS);
	safe_mutex_handle(&philo->philo_mutex, UNLOCK);
	write_status(EATING, philo, DEBUG_MODE);
	if (!safe_sleep(philo, philo->table->time_to_eat))
	{
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
		return ;
	}
	if (philo->table->nbr_limit_meals > 0 &&
		philo->meals_counter == philo->table->nbr_limit_meals)
		set_bool(&philo->philo_mutex, &philo->is_full, true);
	safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
	safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *) data;
	wait_all_threads(philo->table);
	if (!set_long(&philo->philo_mutex, &philo->last_meal_time,
			get_time(MILLISECONDS)))
		return (NULL);
	if (!increase_long(&philo->table->table_mutex,
			&philo->table->threads_running_nbr))
		return (NULL);
	if (philo->id % 2 == 0)
		usleep(100);
	while (!simulation_finished(philo->table))
	{
		if (get_bool(&philo->philo_mutex, &philo->is_full))
			break ;
		if (get_bool(&philo->philo_mutex, &philo->is_dead))
		{
			write_status(DIED, philo, DEBUG_MODE);
			break ;
		}
		eat(philo);
		if (get_bool(&philo->philo_mutex, &philo->is_dead))
		{
			write_status(DIED, philo, DEBUG_MODE);
			break ;
		}
		write_status(SLEEPING, philo, DEBUG_MODE);
		if (!safe_sleep(philo, philo->table->time_to_sleep))
		{
			write_status(DIED, philo, DEBUG_MODE);
			break;
		}
		write_status(THINKING, philo, DEBUG_MODE);
	}
	return (NULL);
}

bool	dinner_start(t_table *table)
{
	int		i;

	if (table->nbr_limit_meals == 0)
		return (false);
	if (table->nbr_philo == 1)
	{
		if (!safe_thread_handle(&table->philos[0].thread_id, lone_philo,
				&table->philos[0], CREATE))
			return (false);
	}
	else
	{
		i = -1;
		while (++i < table->nbr_philo)
		{
			if (!safe_thread_handle(&table->philos[i].thread_id,
					dinner_simulation, &table->philos[i], CREATE))
				return (false);
		}
	}
	if (!safe_thread_handle(&table->monitor, monitor_dinner, table, CREATE))
		return (false);
	if (!safe_mutex_handle(&table->table_mutex, LOCK))
		return (false);
	table->start_time = get_time(MILLISECONDS);
	while (++i < table->nbr_philo)
	{
		if (!set_long(&table->philos[i].philo_mutex,
				&table->philos[i].last_meal_time, table->start_time))
		{
			safe_mutex_handle(&table->table_mutex, UNLOCK);
			return (false);
		}
	}
	if (!safe_mutex_handle(&table->table_mutex, UNLOCK))
		return (false);
	if (!set_bool(&table->table_mutex, &table->are_threads_ready, true))
		return (false);
	i = -1;
	while (++i < table->nbr_philo)
	{
		if (!safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN))
			return (false);
	}
	if (!set_bool(&table->table_mutex, &table->is_finished, true))
		return (false);
	if (!safe_thread_handle(&table->monitor, NULL, NULL, JOIN))
		return (false);
	return (true);
}

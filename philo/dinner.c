/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:23:27 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/04 16:35:59 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	think(t_philo *philo)
{
	write_status(THINKING, philo, DEBUG_MODE);
}
static void	eat(t_philo *philo)
{
	safe_mutex_handle(&philo->left_fork->fork, LOCK);
	write_status(TAKE_LEFT_FORK, philo, DEBUG_MODE);
	safe_mutex_handle(&philo->right_fork->fork, LOCK);
	write_status(TAKE_RIGHT_FORK, philo, DEBUG_MODE);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECONDS));
	philo->meals_counter++;
	write_status(EATING, philo, DEBUG_MODE);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nbr_limit_meals > 0
		&& philo->meals_counter == philo->table->nbr_limit_meals)
		set_bool(&philo->philo_mutex, &philo->is_full, true);
	safe_mutex_handle(&philo->left_fork->fork, UNLOCK);
	safe_mutex_handle(&philo->right_fork->fork, UNLOCK);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *) data;
	wait_all_threads(philo->table);
	increase_long(&philo->table->table_mutex, &philo->table->threads_running_nbr);
	while (!simulation_finished(philo->table))
	{
		// 1) am I full?
		if (philo->is_full)
			break;
		// 2) eat
		eat(philo);
		// 3) sleep -> write status
		write_status(SLEEPING, philo, DEBUG_MODE);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		// 4) think
		think(philo);
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
		; // TO_DO
	else
	{
		while (++i < table->nbr_philo)
		{
			safe_thread_handle(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
			ft_printf("Philo %l is ready.\n", table->philos[i].id);
		}
	}
	safe_thread_handle(&table->monitor, monitor_dinner, table, CREATE);
	table->start_time = get_time(MILLISECONDS);
	set_bool(&table->table_mutex, &table->are_threads_ready, true);
	i = -1;
	while (++i < table->nbr_philo)
	{
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
	}
}

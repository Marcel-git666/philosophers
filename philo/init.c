/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:50:14 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/29 17:23:16 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	int		philo_nbr;

	philo_nbr = philo->table->nbr_philo;
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[(pos + 1) % philo_nbr];
		philo->second_fork = &forks[pos];
	}
	else
	{
		philo->first_fork = &forks[pos];
		philo->second_fork = &forks[(pos + 1) % philo_nbr];
	}
}

static bool	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->nbr_philo)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->is_full = false;
		philo->meals_counter = 0;
		philo->is_dead = false;
		philo->table = table;
		if (!safe_mutex_handle(&philo->philo_mutex, INIT))
		{
			error_message("Failed to initialize philosopher mutex.");
			return (false);
		}
		assign_forks(philo, table->forks, i);
	}
	return (true);
}

bool	init_data(t_table *table)
{
	int		i;

	i = 0;
	table->is_finished = 0;
	table->are_threads_ready = 0;
	table->threads_running_nbr = 0;
	table->is_philo_dead = false;
	table->philos = safe_malloc(table->nbr_philo * sizeof(t_philo));
	table->forks = safe_malloc(table->nbr_philo * sizeof(t_fork));
	if (!table->philos || !table->forks)
		return (false);
	if (!safe_mutex_handle(&table->table_mutex, INIT) ||
		!safe_mutex_handle(&table->write_mutex, INIT))
		return (false);

	while (i < table->nbr_philo)
	{
		if (!safe_mutex_handle(&table->forks[i].fork, INIT))
			return (false);
		table->forks[i].fork_id = i;
		i++;
	}
	if (!init_philo(table))
		return (false);
	return (true);
}

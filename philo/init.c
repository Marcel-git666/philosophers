/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:50:14 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/05 14:00:18 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	int		philo_nbr;

	philo_nbr = philo->table->nbr_philo;
	philo->left_fork = &forks[(pos + 1) % philo_nbr];
	philo->right_fork = &forks[pos];
	if (philo->id % 2 == 0)
	{
		philo->left_fork = &forks[pos];
		philo->right_fork = &forks[(pos + 1) % philo_nbr];
	}
}

static void	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->nbr_philo)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->is_full = 0;
		philo->meals_counter = 0;
		philo->table = table;
		safe_mutex_handle(&philo->philo_mutex, INIT);
		assign_forks(philo, table->forks, i);
	}
}

void	init_data(t_table *table)
{
	int		i;

	i = 0;
	table->is_finished = 0;
	table->are_threads_ready = 0;
	table->threads_running_nbr = 0;
	table->philos = safe_malloc(table->nbr_philo * sizeof(t_philo));
	table->forks = safe_malloc(table->nbr_philo * sizeof(t_fork));
	safe_mutex_handle(&table->table_mutex, INIT);
	safe_mutex_handle(&table->write_mutex, INIT);
	while (i < table->nbr_philo)
	{
		safe_mutex_handle(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
		i++;
	}
	init_philo(table);
	write(1, "Init has finished.\n", 19);
}

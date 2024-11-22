/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:50:14 by mmravec           #+#    #+#             */
/*   Updated: 2024/11/18 17:51:17 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	philo->right_fork = &forks[pos];
	philo->left_fork = &forks[(pos + 1) % philo->table->nbr_philo];
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
		assign_forks(philo, table->forks, i);
	}

}

void	init_data(t_table *table)
{
	int		i;

	i = 0;
	table->is_finished = 0;
	table->are_threads_ready = 0;
	table->philos = safe_malloc(table->nbr_philo * sizeof(t_philo));
	table->forks = safe_malloc(table->nbr_philo * sizeof(t_fork));
	safe_mutex_handle(&table->table_mutex, INIT);
	while (i < table->nbr_philo)
	{
		safe_mutex_handle(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
		i++;
	}
	init_philo(table);
}

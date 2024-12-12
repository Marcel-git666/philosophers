/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:50:14 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/12 15:24:57 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->nbr_philo)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->meals_counter = 0;
		philo->table = table;
	}
}

void	init_data(t_table *table)
{
	table->is_finished = false;
	table->philos = safe_malloc(table->nbr_philo * sizeof(t_philo));
	table->forks = safe_semaphore_handle("/forks_sem", table->nbr_philo,
			SEM_OPEN, NULL);
	table->write_sem = safe_semaphore_handle("/write_sem", 1, SEM_OPEN, NULL);
	table->start_sem = safe_semaphore_handle("/start_sem", 0, SEM_OPEN, NULL);
	init_philo(table);
}

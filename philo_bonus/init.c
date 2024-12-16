/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:50:14 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/16 12:02:35 by mmravec          ###   ########.fr       */
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
		philo->last_meal_time = get_time(MILLISECONDS);
		philo->table = table;
	}
}

void	init_data(t_table *table)
{
	table->philos = safe_malloc(table->nbr_philo * sizeof(t_philo));
	table->forks = safe_semaphore_handle(FORKS_SEM, table->nbr_philo,
			SEM_OPEN, NULL);
	table->write_sem = safe_semaphore_handle(WRITE_SEM, 1, SEM_OPEN, NULL);
	table->start_sem = safe_semaphore_handle(START_SEM, 0, SEM_OPEN, NULL);
	table->finished_sem = safe_semaphore_handle(FINISHED_SEM, 0, SEM_OPEN, NULL);
	init_philo(table);
}

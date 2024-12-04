/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:23:27 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/04 14:19:07 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *) data;
	wait_all_threads(philo->table);

	while (!simulation_finished(philo->table))
	{
		// 1) am I full?
		if (philo->is_full)
			break;
		// 2) eat
		eat(philo);
		// 3) sleep -> write status
		// 4) think
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
			printf("Philo %d is ready.\n", table->philos[i].id);
		}
	}
	table->start_time = get_time(MILLISECONDS);
	set_bool(&table->table_mutex, &table->are_threads_ready, true);
	i = -1;
	while (++i < table->nbr_philo)
	{
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
	}
}

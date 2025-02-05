/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:50:14 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/05 16:59:40 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	create_last_meal_sem_name(char *buffer, int id)
{
	int		i;
	char	num[12];

	buffer[0] = '/';
	buffer[1] = 'l';
	buffer[2] = 'a';
	buffer[3] = 's';
	buffer[4] = 't';
	buffer[5] = '_';
	buffer[6] = 'm';
	buffer[7] = 'e';
	buffer[8] = 'a';
	buffer[9] = 'l';
	buffer[10] = '_';
	i = 0;
	while (id > 0)
	{
		num[i++] = '0' + (id % 10);
		id /= 10;
	}
	while (--i >= 0)
		buffer[11 + i] = num[i];
	buffer[11 + i + 1] = '\0';
}

static void	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;
	char	last_meal_sem_name[20];

	i = -1;
	while (++i < table->nbr_philo)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->meals_counter = 0;
		philo->last_meal_time = get_time(MILLISECONDS);
		philo->table = table;
		create_last_meal_sem_name(last_meal_sem_name, philo->id);
		philo->last_meal_sem = safe_semaphore_handle(last_meal_sem_name,
				1, SEM_OPEN, NULL);
	}
}

void	init_data(t_table *table)
{
	table->philos = safe_malloc(table->nbr_philo * sizeof(t_philo));
	table->forks = safe_semaphore_handle(FORKS_SEM, table->nbr_philo,
			SEM_OPEN, NULL);
	table->write_sem = safe_semaphore_handle(WRITE_SEM, 1, SEM_OPEN, NULL);
	table->start_sem = safe_semaphore_handle(START_SEM, 0, SEM_OPEN, NULL);
	table->death_sem = safe_semaphore_handle(DEATH_SEM, 0, SEM_OPEN, NULL);
	table->start_time_sem = safe_semaphore_handle(START_TIME_SEM, 1,
			SEM_OPEN, NULL);
	table->all_full_sem = safe_semaphore_handle(ALL_FULL_SEM, 0, SEM_OPEN,
			NULL);
	init_philo(table);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deinit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:39:09 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/05 17:00:26 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	clean_philos(t_table *table)
{
	int		i;
	char	last_meal_sem_name[20];

	i = -1;
	while (++i < table->nbr_philo)
	{
		create_last_meal_sem_name(last_meal_sem_name, i + 1);
		if (table->philos[i].last_meal_sem)
		{
			safe_semaphore_handle(NULL, 0, SEM_CLOSE,
				table->philos[i].last_meal_sem);
			if (sem_unlink(last_meal_sem_name) == -1 && errno != ENOENT)
			{
				ft_printf("Failed to unlink semaphore: %s, ERR: %s\n",
					last_meal_sem_name, strerror(errno));
			}
		}
	}
}

void	clean(t_table *table)
{
	write(1, "Cleaning philos....\n", 20);
	clean_philos(table);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(WRITE_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(START_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(DEATH_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(ALL_FULL_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_CLOSE, table->forks);
	safe_semaphore_handle(WRITE_SEM, 0, SEM_CLOSE, table->write_sem);
	safe_semaphore_handle(START_SEM, 0, SEM_CLOSE, table->start_sem);
	safe_semaphore_handle(START_TIME_SEM, 0, SEM_CLOSE, table->start_time_sem);
	safe_semaphore_handle(DEATH_SEM, 0, SEM_CLOSE, table->death_sem);
	safe_semaphore_handle(ALL_FULL_SEM, 0, SEM_CLOSE, table->all_full_sem);
	free(table->philos);
}

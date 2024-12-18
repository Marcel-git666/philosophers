/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:25:28 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/18 16:45:21 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean(t_table *table)
{
	int		i;

	if (!table || !table->philos)
		return ;
	i = -1;
	while (++i < table->nbr_philo)
	{
		if (table->philos[i].meals_sem)
		{
			safe_semaphore_handle(NULL, 0, SEM_CLOSE,
				table->philos[i].meals_sem);
			safe_semaphore_handle(table->philos[i].sem_name, 0, SEM_UNLINK,
				NULL);
		}
		free(table->philos[i].sem_name);
	}
	safe_semaphore_handle(FORKS_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(WRITE_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(START_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(DEATH_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(ALL_FULL_SEM, 0, SEM_UNLINK, NULL);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_CLOSE, table->forks);
	safe_semaphore_handle(WRITE_SEM, 0, SEM_CLOSE, table->write_sem);
	safe_semaphore_handle(START_SEM, 0, SEM_CLOSE, table->start_sem);
	safe_semaphore_handle(DEATH_SEM, 0, SEM_CLOSE, table->death_sem);
	safe_semaphore_handle(ALL_FULL_SEM, 0, SEM_CLOSE, table->all_full_sem);
	free(table->philos);
}

void	error_exit(const char *error)
{
	write(2, "Error\n", 6);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

long	get_time(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit("Get time failure.");
	if (time_code == SECONDS)
		return (tv.tv_sec + tv.tv_usec / 1e6);
	else if (time_code == MILLISECONDS)
		return (tv.tv_sec * 1000 + tv.tv_usec / 1e3);
	else if (time_code == MICROSECONDS)
		return (tv.tv_sec * 1e6 + tv.tv_sec);
	else
		error_exit("Wrong input for gettime.");
	return (-1);
}

// void	precise_usleep(long usec, t_table *table)
// {
// 	long	start;
// 	long	elapsed;
// 	long	remaining;

// 	start = get_time(MICROSECONDS);
// 	while (get_time(MICROSECONDS) - start < usec)
// 	{
// 		if (table->is_finished)
// 			break ;
// 		elapsed = get_time(MICROSECONDS) - start;
// 		remaining = usec - elapsed;
// 		if (remaining > 1e3)
// 			usleep(remaining / 2);
// 		else
// 			usleep(50);
// 			// while (get_time(MICROSECONDS) - start < usec)
// 			// 	;
// 	}
// }

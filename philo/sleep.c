/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:45:04 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/29 19:01:18 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	safe_sleep(t_philo *philo, long msec)
{
	long	start;
	long	time_t_die;

	start = get_time(MILLISECONDS);
	time_t_die = (*philo).last_meal_time + (*philo).table->time_to_die - start;
	if (time_t_die > msec)
	{
		usleep(msec * 1000);
		return (true);
	}
	usleep(time_t_die * 1000);
	philo->is_dead = true;
	if (!safe_mutex_handle(&philo->table->table_mutex, LOCK))
		return (false);
	philo->table->is_philo_dead = true;
	if (!safe_mutex_handle(&philo->table->table_mutex, UNLOCK))
		error_message("Failed to unlock table_mutex in safe_sleep.");
	return (false);
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

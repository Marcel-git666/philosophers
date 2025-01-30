/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:45:04 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/30 08:13:08 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	safe_sleep(t_philo *philo, long msec)
{
	long	start;
	long	elapsed;
	long	check_interval;

	start = get_time(MILLISECONDS);
	elapsed = 0;
	check_interval = 1000; // Kontrola každou 1 ms

	while (elapsed < msec)
	{
		if (get_bool(&philo->table->table_mutex, &philo->table->is_finished))  // 💡 Okamžité ukončení
			return (false);

		elapsed = get_time(MILLISECONDS) - start;

		// Kontrola, zda filozof neumřel
		if (get_time(MILLISECONDS) - get_long(&philo->philo_mutex, &philo->last_meal_time) > philo->table->time_to_die)
		{
			set_bool(&philo->philo_mutex, &philo->is_dead, true);
			set_bool(&philo->table->table_mutex, &philo->table->is_finished, true);
			return (false);
		}

		usleep(check_interval);
	}
	return (true);
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

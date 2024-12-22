/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:45:04 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/22 15:34:04 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_sleep(t_philo *philo, long msec)
{
	long	start;
	long	time_t_die;

	start = get_time(MILLISECONDS);
	time_t_die = (*philo).last_meal_time + (*philo).table->time_to_die - start;
	if (time_t_die > msec)
	{
		usleep(msec * 1000);
		return ;
	}
	usleep(time_t_die * 1000);

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

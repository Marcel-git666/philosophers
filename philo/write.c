/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:35:51 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/05 14:59:24 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	write_status_debug(t_philo_status status, t_philo *philo,
		long elapsed)
{
	if (status == TAKE_LEFT_FORK && !simulation_finished(philo->table))
		ft_printf("%l %d has taken left fork - %d.\n", elapsed, philo->id,
			philo->left_fork->fork_id);
	else if (status == TAKE_RIGHT_FORK && !simulation_finished(philo->table))
		ft_printf("%l %d has taken right fork.\n", elapsed, philo->id);
	else if (status == EATING && !simulation_finished(philo->table))
		ft_printf("%l %d is eating for %d ms \t\t\t Meal number: %d.\n",
			elapsed, philo->id, philo->table->time_to_eat,
			philo->meals_counter);
	else if (status == SLEEPING && !simulation_finished(philo->table))
		ft_printf("%l %d is sleeping.\n", elapsed, philo->id);
	else if (status == THINKING && !simulation_finished(philo->table))
		ft_printf("%l %d is thinking.\n", elapsed, philo->id);
	else if (status == DIED)
		ft_printf("%l %d has died.\n", elapsed, philo->id);
}

void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	elapsed;

	elapsed = get_time(MILLISECONDS) - philo->table->start_time;
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
	{
		if ((status == TAKE_LEFT_FORK || status == TAKE_RIGHT_FORK)
			&& !simulation_finished(philo->table))
			ft_printf("%l %d has taken a fork.\n", elapsed, philo->id);
		else if (status == EATING && !simulation_finished(philo->table))
			ft_printf("%l %d is eating.\n", elapsed, philo->id);
		else if (status == SLEEPING && !simulation_finished(philo->table))
			ft_printf("%l %d is sleeping.\n", elapsed, philo->id);
		else if (status == THINKING && !simulation_finished(philo->table))
			ft_printf("%l %d is thinking.\n", elapsed, philo->id);
		else if (status == DIED)
			ft_printf("%l %d has died.\n", elapsed, philo->id);
	}
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}

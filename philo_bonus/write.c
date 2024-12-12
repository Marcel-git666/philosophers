/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:35:51 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/12 16:07:11 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	write_status_debug(t_philo_status status, t_philo *philo,
		long elapsed)
{
	if (status == TAKE_FIRST_FORK)
		ft_printf("%l ms Philo: %d has taken a fork.\n", elapsed, philo->id);
	else if (status == TAKE_SECOND_FORK)
		ft_printf("%l ms Philo: %d has taken a fork.\n", elapsed, philo->id);
	else if (status == EATING)
		ft_printf("%l ms Philo: %d is eating for %l ms \t\t\t Meal number: %l.\n",
			elapsed, philo->id, philo->table->time_to_eat, philo->meals_counter);
	else if (status == SLEEPING)
		ft_printf("%l ms Philo: %d is sleeping for %l ms.\n", elapsed, philo->id,
			philo->table->time_to_sleep);
	else if (status == THINKING)
		ft_printf("%l ms Philo: %d is thinking.\n", elapsed, philo->id);
	else if (status == DIED)
		ft_printf("%l ms Philo: %d has died.\n", elapsed, philo->id);
}

void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	elapsed;

	elapsed = get_time(MILLISECONDS) - philo->table->start_time;
	safe_semaphore_handle(NULL, 0, SEM_WAIT, philo->table->write_sem);
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
	{
		if (status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
			ft_printf("%l %d has taken a fork\n", elapsed, philo->id);
		else if (status == EATING)
			ft_printf("%l %d is eating\n", elapsed, philo->id);
		else if (status == SLEEPING)
			ft_printf("%l %d is sleeping\n", elapsed, philo->id);
		else if (status == THINKING)
			ft_printf("%l %d is thinking\n", elapsed, philo->id);
		else if (status == DIED)
			ft_printf("%l %d died\n", elapsed, philo->id);
	}
	safe_semaphore_handle(NULL, 0, SEM_POST, philo->table->write_sem);
}

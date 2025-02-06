/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 08:28:50 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/06 08:56:15 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eat(t_philo *philo)
{
	safe_semaphore_handle(FORKS_SEM, 0, SEM_WAIT, philo->table->forks);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_WAIT, philo->table->forks);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	safe_semaphore_handle(WRITE_SEM, 0, SEM_WAIT, philo->table->write_sem);
	philo->last_meal_time = get_time(MILLISECONDS);
	philo->meals_counter++;
	safe_semaphore_handle(WRITE_SEM, 0, SEM_POST, philo->table->write_sem);
	write_status(EATING, philo, DEBUG_MODE);
	usleep(philo->table->time_to_eat * 1000);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_POST, philo->table->forks);
	safe_semaphore_handle(FORKS_SEM, 0, SEM_POST, philo->table->forks);
}

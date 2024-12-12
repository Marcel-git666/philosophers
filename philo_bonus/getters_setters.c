/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:57:05 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/12 16:13:07 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// void	set_simulation_finished(t_table *table, bool value)
// {
// 	// sem_wait(table->write_sem);  // Block other processes
// 	// table->is_finished = value; // Modify the shared variable
// 	sem_post(table->write_sem); // Release semaphore
// }

// bool	simulation_finished(t_table *table)
// {
// 	bool	is_finished;

// 	sem_wait(table->write_sem);
// 	is_finished = table->is_finished;
// 	sem_post(table->write_sem);
// 	return (is_finished);
// }

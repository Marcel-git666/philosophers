/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:10:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/29 17:46:51 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_table *table)
{
	while (!get_bool(&table->table_mutex, &table->are_threads_ready))
		usleep(100);
}

bool	all_threads_are_running(t_mtx *mutex, long *threads, long philo_nbr)
{
	bool	ret;

	ret = false;
	if (!safe_mutex_handle(mutex, LOCK))
		return (false);
	if (*threads == philo_nbr)
		ret = true;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (false);
	return (ret);
}

bool	increase_long(t_mtx *mutex, long *value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (false);
	(*value)++;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (false);
	return (true);
}

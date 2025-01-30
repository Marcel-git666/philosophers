/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:57:05 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/30 10:06:22 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_bool(t_mtx *mutex, bool *dest, bool value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return ;
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		error_message("Failed to unlock mutex in set_bool.");
}

bool	get_bool(t_mtx *mutex, bool *value)
{
	bool	ret;

	if (!safe_mutex_handle(mutex, LOCK))
		return (false);
	ret = *value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		error_message("Failed to unlock mutex in get_bool.");
	return (ret);
}

void	set_long(t_mtx *mutex, long *dest, long value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return ;
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		error_message("Failed to unlock mutex in set_long.");
}

long	get_long(t_mtx *mutex, long *value)
{
	long	ret;

	ret = -1;
	if (!safe_mutex_handle(mutex, LOCK))
		return (ret);
	ret = *value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		error_message("Failed to unlock mutex in get_long.");
	return (ret);
}

bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->is_finished));
}

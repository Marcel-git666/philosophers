/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:57:05 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/29 19:21:42 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	set_bool(t_mtx *mutex, bool *dest, bool value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (false);
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
	{
		error_message("Failed to unlock mutex in set_bool.");
		return (false);
	}
	return (true);
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

bool	set_long(t_mtx *mutex, long *dest, long value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (false);
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
	{
		error_message("Failed to unlock mutex in set_long.");
		return (false);
	}
	return (true);
}

long	get_long(t_mtx *mutex, long *value)
{
	long	ret;

	ret = 0;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:25:28 by mmravec           #+#    #+#             */
/*   Updated: 2025/01/30 10:05:35 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

void	error_message(const char *error)
{
	write(2, "Error\n", 6);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

long	get_time(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_message("Get time failure.");
	if (time_code == SECONDS)
		return (tv.tv_sec + tv.tv_usec / 1e6);
	else if (time_code == MILLISECONDS)
		return (tv.tv_sec * 1000 + tv.tv_usec / 1e3);
	else if (time_code == MICROSECONDS)
		return (tv.tv_sec * 1e6 + tv.tv_usec);
	else
		error_message("Wrong input for gettime.");
	return (-1);
}


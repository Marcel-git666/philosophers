/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:25:05 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/10 15:31:44 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(const char *str)
{
	long	result;
	long	positive;

	positive = 1;
	result = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			positive = positive * (-1);
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * positive);
}

void	parse_input(t_table *table, char **argv)
{
	table->nbr_philo = ft_atoi(argv[1]);
	// ft_printf("Nbr: %l\n", table->nbr_philo);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5]) {
		table->nbr_limit_meals = ft_atoi(argv[5]);
		if (table->nbr_limit_meals < 1)
			error_exit("Last argument can't be lower than 1.");
	}
	else
		table->nbr_limit_meals = -1;
}

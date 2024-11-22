/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2024/11/18 17:23:17 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	*routine()
{

	return NULL;
}

// int argc, char **argv
int	main(int argc, char **argv)
{
	t_table	table;

	if (argc == 5 || argc == 6)
	{
		parse_input(&table, argv);
		if (table.nbr_philo > 200)
			error_exit("Too many philosophers (200 is max)");
		init_data(&table);
		make_dinner(&table);
	}
	else
		error_exit("Wrong number of arguments. ./philo 5 100 50 20 [7]");
	return (0);
}

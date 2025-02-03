/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/03 09:12:57 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc == 5 || argc == 6)
	{
		if (!parse_input(&table, argv))
			return (1);
		if (table.nbr_philo > 200)
			ft_printf("Too many philosophers (200 is max), "
				"unexpected results.");
		if (table.time_to_die < 1 || table.time_to_eat < 1
			|| table.time_to_sleep < 1 || table.nbr_philo < 1)
		{
			error_exit("Wrong arguments. ./philo 5 100 50 20 [7]");
			return (1);
		}
		init_data(&table);
		dinner_start(&table);
		clean(&table);
	}
	else
		error_exit("Wrong number of arguments. ./philo 5 100 50 20 [7]");
	return (0);
}

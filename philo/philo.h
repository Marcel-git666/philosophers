/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2024/10/31 19:40:42 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo_params
	{
		int	number_of_philosophers;
		int	time_to_die;
		int	time_to_eat;
		int	time_to_sleep;
		int	number_of_times_each_philosopher_must_eat;
	}	t_philo_params;

typedef struct s_philo
	{
		int	id;
		int	time_from_last_meal;
		int	is_dead;

	}	t_philo

int	ft_atoi(const char *str);

#endif

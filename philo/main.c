/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2024/10/31 15:54:05 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	*routine(void *arg)
{
	pthread_mutex_t *mutex = ((pthread_mutex_t **)arg)[0];
	int *mails = ((int **)arg)[1];

	for (int i = 0; i < 10000000; i++) {
		pthread_mutex_lock(mutex);
		(*mails)++;
		pthread_mutex_unlock(mutex);
	}
	return NULL;
}

// int argc, char **argv
int	main(int argc, char **argv)
{
	pthread_mutex_t mutex;
	int				mails;
	void			*args[2];
	pthread_t *philosophers;
	int		i;
	t_philo_params params;

	mails = 0;
	args[0] = &mutex;
	args[1] = &mails;
	if (argc == 5 || argc == 6)
	{
		params.number_of_philosophers = ft_atoi(argv[1]);
		params.time_to_die = ft_atoi(argv[2]);
		params.time_to_eat = ft_atoi(argv[3]);
		params.time_to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			params.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
		else
			params.number_of_times_each_philosopher_must_eat = -1;
	}
	else
		return (0);
	pthread_mutex_init(&mutex, NULL);
	philosophers = malloc(params.number_of_philosophers * sizeof(pthread_t));
	for (i = 0; i < params.number_of_philosophers; i++)
	{
		if (pthread_create(philosophers + i, NULL, &routine, args) != 0)
		{
			perror("Couldn't create a thread.\n");
			return (1);
		}
		printf("Thread %d has started.\n", i);
	}
	for (i = 0; i < params.number_of_philosophers; i++)
	{
		if (pthread_join(philosophers[i], NULL) != 0)
		{
			return 2;
		}
		printf("Thread %d has finished execution.\n", i);
	}
	pthread_mutex_destroy(&mutex);
	printf("Number of mails: %d\n", mails);
	return 0;
}

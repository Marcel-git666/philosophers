/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:50:14 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/18 16:39:25 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*create_sem_name(int philo_id)
{
	char	*num;
	char	*name;
	char	*prefix;

	prefix = "/p_";
	num = ft_itoa(philo_id);
	if (!num)
		return (NULL);
	name = malloc(ft_strlen(prefix) + ft_strlen(num) + 1);
	if (!name)
	{
		free(num);
		return (NULL);
	}
	ft_strlcpy(name, prefix, ft_strlen(prefix) + 1);
	ft_strlcat(name, num, ft_strlen(prefix) + ft_strlen(num) + 1);
	free(num);
	return (name);
}

static void	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->nbr_philo)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->meals_counter = 0;
		philo->last_meal_time = get_time(MILLISECONDS);
		philo->sem_name = create_sem_name(philo->id);
		if (!philo->sem_name)
			error_exit("Memory allocation failed");
		philo->meals_sem = safe_semaphore_handle(
				philo->sem_name, 1, SEM_OPEN, NULL);
		philo->table = table;
	}
}

void	init_data(t_table *table)
{
	table->philos = safe_malloc(table->nbr_philo * sizeof(t_philo));
	table->forks = safe_semaphore_handle(FORKS_SEM, table->nbr_philo,
			SEM_OPEN, NULL);
	table->write_sem = safe_semaphore_handle(WRITE_SEM, 1, SEM_OPEN, NULL);
	table->start_sem = safe_semaphore_handle(START_SEM, 0, SEM_OPEN, NULL);
	table->death_sem = safe_semaphore_handle(DEATH_SEM, 0, SEM_OPEN, NULL);
	table->all_full_sem = safe_semaphore_handle(
			ALL_FULL_SEM, 0, SEM_OPEN, NULL);
	init_philo(table);
}

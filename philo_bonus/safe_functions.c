/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:55:05 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/11 15:53:03 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (ret == NULL)
		error_exit("Malloc error.");
	return (ret);
}

sem_t	*safe_semaphore_handle(const char *name, int initial_value,
			t_sem_opcode opcode, sem_t *sem)
{
	sem_t	*ret;

	ret = NULL;
	if (opcode == SEM_OPEN)
	{
		ret = sem_open(name, O_CREAT | O_EXCL, 0644, initial_value);
		if (ret == SEM_FAILED)
			error_exit("Semaphore open error");
	}
	else if (opcode == SEM_WAIT)
		if (sem_wait(sem) == -1)
			error_exit("Semaphore wait error");
	else if (opcode == SEM_POST)
		if (sem_post(sem) == -1)
			error_exit("Semaphore post error");
	else if (opcode == SEM_CLOSE)
		if (sem_close(sem) == -1)
			error_exit("Semaphore close error");
	else if (opcode == SEM_UNLINK)
		if (sem_unlink(name) == -1)
			error_exit("Semaphore unlink error");
	else
		error_exit("Invalid semaphore operation.\n");
	return (ret);
}

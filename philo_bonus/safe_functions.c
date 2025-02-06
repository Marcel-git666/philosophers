/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:55:05 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/06 08:49:19 by mmravec          ###   ########.fr       */
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

static sem_t	*safe_semaphore_open(const char *name, int initial_value)
{
	sem_t	*sem;

	sem = sem_open(name, O_CREAT | O_EXCL, 0644, initial_value);
	if (sem == SEM_FAILED && errno == EEXIST)
	{
		sem_unlink(name);
		sem = sem_open(name, O_CREAT | O_EXCL, 0644, initial_value);
		if (sem == SEM_FAILED)
			error_exit("Semaphore open error after unlink");
	}
	else if (sem == SEM_FAILED)
		error_exit("Semaphore open error");
	return (sem);
}

static void	safe_semaphore_op(t_sem_opcode opcode, sem_t *sem, const char *name)
{
	if ((opcode == SEM_WAIT && sem_wait(sem) == -1)
		|| (opcode == SEM_POST && sem_post(sem) == -1)
		|| (opcode == SEM_CLOSE && sem_close(sem) == -1)
		|| (opcode == SEM_UNLINK && sem_unlink(name) == -1))
	{
		error_exit("Semaphore operation error");
	}
}

sem_t	*safe_semaphore_handle(const char *name, int initial_value,
			t_sem_opcode opcode, sem_t *sem)
{
	if (opcode == SEM_OPEN)
		return (safe_semaphore_open(name, initial_value));
	safe_semaphore_op(opcode, sem, name);
	return (NULL);
}

void	safe_process_handle(pid_t *process_id, void *data, void (*f)(void *),
			t_opcode opcode)
{
	if (opcode == CREATE)
	{
		*process_id = fork();
		if (*process_id < 0)
			error_exit("Fork failed.");
		if (*process_id == 0)
		{
			f(data);
			exit(0);
		}
	}
	else if (opcode == KILL)
	{
		if (kill(*process_id, SIGKILL) < 0)
			error_exit("Failed to kill process.");
	}
	else if (opcode == WAIT)
	{
		if (waitpid(*process_id, NULL, 0) < 0)
			error_exit("Failed to wait for process.");
	}
	else
		error_exit("Invalid process operation.");
}

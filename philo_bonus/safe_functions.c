/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:55:05 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/05 12:10:04 by mmravec          ###   ########.fr       */
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
		ft_printf("Semaphore operation error: %s (OP: %d, ERR: %s)\n",
			name ? name : "Unnamed", opcode, strerror(errno));
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


static void	handle_pthread_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (status == EAGAIN)
		error_exit("The system lacked the necessary resources to create another"
			" thread, or the system-imposed limit on the total number of "
			"threads in a process [PTHREAD_THREADS_MAX] would be exceeded.");
	else if (status == EPERM)
		error_exit("The caller does not have appropriate permission to set the"
			" required scheduling parameters or scheduling policy.");
	else if (status == EINVAL && opcode == CREATE)
		error_exit("The value specified by attr is invalid.");
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		error_exit("The implementation has detected that the value specified by"
			" thread does not refer to a joinable thread.");
	else if (status == ESRCH)
		error_exit("No thread could be found corresponding to that specified by"
			" the given thread ID, thread.");
	else if (status == EDEADLK)
		error_exit("A deadlock was detected or the value of thread specifies"
			" the calling thread.");
}

void	safe_thread_handle(pthread_t *thread, void *(*f)(void *), void *data,
			t_opcode opcode)
{
	if (opcode == CREATE)
		handle_pthread_error(pthread_create(thread, NULL, f, data), opcode);
	else if (opcode == JOIN)
		handle_pthread_error(pthread_join(*thread, NULL), opcode);
	else if (opcode == DETACH)
		handle_pthread_error(pthread_detach(*thread), opcode);
	else
		error_exit("Wrong opcode, use CREATE, JOIN or DETACH");
}

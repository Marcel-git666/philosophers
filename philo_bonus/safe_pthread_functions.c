/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pthread_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 08:45:32 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/06 08:55:34 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resources.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 00:17:31 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/04 21:18:04 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_common.h"

static int	get_semaphore(key_t key, bool create)
{
	int	sem_id;

	sem_id = semget(key, 1, 0666);
	if (sem_id == -1 && create)
	{
		sem_id = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
		if (sem_id == -1)
			return (-1);
		if (semctl(sem_id, 0, SETVAL, 1) == -1)
		{
			semctl(sem_id, 0, IPC_RMID);
			return (-1);
		}
	}
	return (sem_id);
}

static int	get_shared_memory(key_t key, bool create)
{
	int	shm_id;

	shm_id = shmget(key, sizeof(t_shared_data), 0666);
	if (shm_id == -1 && create)
	{
		shm_id = shmget(key, sizeof(t_shared_data),
				0666 | IPC_CREAT | IPC_EXCL);
		if (shm_id == -1)
			return (-1);
	}
	return (shm_id);
}

static int	get_message_queue(key_t key, bool create)
{
	int	msq_id;

	msq_id = msgget(key, 0666);
	if (msq_id == -1 && create)
	{
		msq_id = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
		if (msq_id == -1)
			return (-1);
	}
	return (msq_id);
}

t_shared_resources_ids	*get_shared_resources(bool create)
{
	t_shared_resources_ids	*ids;

	ids = (t_shared_resources_ids *)malloc(sizeof(t_shared_resources_ids));
	ids->key = ftok("shmfile", 65);
	if (ids->key == -1)
	{
		free(ids);
		return (NULL);
	}
	ids->sem_id = get_semaphore(ids->key, create);
	if (ids->sem_id == -1)
	{
		free(ids);
		return (NULL);
	}
	ids->shm_id = get_shared_memory(ids->key, create);
	if (ids->shm_id == -1)
	{
		semctl(ids->sem_id, 0, IPC_RMID);
		free(ids);
		return (NULL);
	}
	ids->msq_id = get_message_queue(ids->key, create);
	if (ids->msq_id == -1)
	{
		semctl(ids->sem_id, 0, IPC_RMID);
		shmctl(ids->shm_id, IPC_RMID, NULL);
		free(ids);
		return (NULL);
	}
	return (ids);
}

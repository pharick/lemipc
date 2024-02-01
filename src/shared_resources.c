/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resources.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 00:17:31 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/01 16:03:05 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

static int	get_semaphore(key_t key)
{
	int	sem_id;

	sem_id = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
	if (sem_id == -1 && errno == EEXIST)
	{
		sem_id = semget(key, 1, 0666);
		return (sem_id);
	}
	if (sem_id == -1)
		return (-1);
	if (semctl(sem_id, 0, SETVAL, 1) == -1)
	{
		semctl(sem_id, 0, IPC_RMID);
		return (-1);
	}
	return (sem_id);
}

static int	get_shared_memory(key_t key)
{
	int	shm_id;

	shm_id = shmget(key, sizeof(t_shared_data), 0666 | IPC_CREAT | IPC_EXCL);
	if (shm_id == -1 && errno == EEXIST)
	{
		shm_id = shmget(key, sizeof(t_shared_data), 0666);
		return (shm_id);
	}
	if (shm_id == -1)
		return (-1);
	return (shm_id);
}

t_shared_resources_ids	*get_shared_resources(void)
{
	t_shared_resources_ids	*ids;

	ids = (t_shared_resources_ids *)malloc(sizeof(t_shared_resources_ids));
	ids->key = ftok("shmfile", 65);
	if (ids->key == -1)
	{
		free(ids);
		return (NULL);
	}
	ids->sem_id = get_semaphore(ids->key);
	if (ids->sem_id == -1)
	{
		free(ids);
		return (NULL);
	}
	ids->shm_id = get_shared_memory(ids->key);
	if (ids->shm_id == -1)
	{
		semctl(ids->sem_id, 0, IPC_RMID);
		free(ids);
		return (NULL);
	}
	return (ids);
}

void	clean_shared_resources(t_shared_resources_ids *ids)
{
	if (ids == NULL)
		return ;
	if (ids->shm_id != -1)
		shmctl(ids->shm_id, IPC_RMID, NULL);
	if (ids->sem_id != -1)
		semctl(ids->sem_id, 0, IPC_RMID);
	free(ids);
}

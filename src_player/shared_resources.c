/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resources.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 00:17:31 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/05 00:36:57 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_player.h"

void	clean_shared_resources(t_shared_resources_ids *ids)
{
	if (ids == NULL)
		return ;
	if (ids->shm_id != -1)
		shmctl(ids->shm_id, IPC_RMID, NULL);
	if (ids->sem_id != -1)
		semctl(ids->sem_id, 0, IPC_RMID);
	if (ids->msq_id != -1)
		msgctl(ids->msq_id, IPC_RMID, NULL);
}

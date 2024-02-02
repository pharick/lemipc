/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resources.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 00:17:31 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 16:22:09 by cbelva           ###   ########.fr       */
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
	free(ids);
}

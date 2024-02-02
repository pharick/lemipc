/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:03:08 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 16:31:51 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_player.h"

void	with_data(int sem_id, t_shared_data *shared_data,
	t_player_data *player_data, t_shared_data_operation op)
{
	struct sembuf	sem_op;

	sem_op.sem_num = 0;
	sem_op.sem_op = -1;
	sem_op.sem_flg = 0;
	semop(sem_id, &sem_op, 1);
	op(shared_data, player_data);
	sem_op.sem_op = 1;
	semop(sem_id, &sem_op, 1);
}

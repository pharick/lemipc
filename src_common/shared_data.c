/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:03:08 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 16:31:42 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_common.h"

t_shared_data	read_shared_data(int sem_id, t_shared_data *shared_data)
{
	t_shared_data	result;
	struct sembuf	sem_op;

	sem_op.sem_num = 0;
	sem_op.sem_op = -1;
	sem_op.sem_flg = 0;
	semop(sem_id, &sem_op, 1);
	result = *shared_data;
	sem_op.sem_op = 1;
	semop(sem_id, &sem_op, 1);
	return (result);
}

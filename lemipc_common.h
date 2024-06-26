

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc_common.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 15:36:34 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/26 18:22:09 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_COMMON_H
# define LEMIPC_COMMON_H

# define MAP_WIDTH 20
# define MAP_HEIGHT 20

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/msg.h>

# include "libft.h"

typedef struct s_shared_data
{
	size_t	nb_players;
	size_t	map[MAP_HEIGHT][MAP_WIDTH];
}	t_shared_data;

typedef struct s_shared_resources_ids
{
	key_t	key;
	int		shm_id;
	int		sem_id;
	int		msq_id;
}	t_shared_resources_ids;

t_shared_resources_ids	*get_shared_resources(bool create);
t_shared_data			read_shared_data(int sem_id,
							t_shared_data *shared_data);
size_t					check_winning_team(const size_t map[MAP_HEIGHT][MAP_WIDTH]);

#endif

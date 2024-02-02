/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 23:22:58 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 12:04:39 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_H
# define LEMIPC_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <stdlib.h>
# include <stdbool.h>
# include <errno.h>
# include <time.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>

# define MAP_WIDTH 10
# define MAP_HEIGHT 10

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
}	t_shared_resources_ids;

typedef struct s_coord
{
	size_t	x;
	size_t	y;
}	t_coord;

typedef struct s_player_data
{
	size_t					team_id;
	t_coord					coord;
}	t_player_data;

typedef void			(*t_shared_data_operation)(t_shared_data *shared_data,
							t_player_data *player_data);

extern bool				g_running;

t_shared_resources_ids	*get_shared_resources(void);
void					clean_shared_resources(t_shared_resources_ids *ids);
void					with_data(int sem_id, t_shared_data *shared_data,
							t_player_data *player_data,
							t_shared_data_operation op);
t_shared_data			read_shared_data(int sem_id,
							t_shared_data *shared_data);

size_t					game_loop(size_t team_id, t_shared_data *shared_data,
							const t_shared_resources_ids *shared_resources_ids);
void					player_turn(t_shared_data *shared_data, t_player_data *player_data);

#endif

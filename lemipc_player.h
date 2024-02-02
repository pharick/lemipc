/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc_player.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 23:22:58 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 18:02:59 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_PLAYER_H
# define LEMIPC_PLAYER_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <time.h>

# include "lemipc_common.h"

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

void					clean_shared_resources(t_shared_resources_ids *ids);
void					with_data(int sem_id, t_shared_data *shared_data,
							t_player_data *player_data,
							t_shared_data_operation op);

size_t					game_loop(size_t team_id, t_shared_data *shared_data,
							const t_shared_resources_ids *shared_resources_ids);
void					player_turn(t_shared_data *shared_data,
							t_player_data *player_data);

#endif

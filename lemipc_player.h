/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc_player.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 23:22:58 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/05 02:07:08 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_PLAYER_H
# define LEMIPC_PLAYER_H

# include <signal.h>
# include <time.h>

# include "lemipc_common.h"

# define MAX_TEAMS 9

typedef struct __attribute__((packed)) s_coord
{
	ssize_t	x;
	ssize_t	y;
}	t_coord;

typedef struct s_player_data
{
	ssize_t	team_id;
	t_coord	coord;
}	t_player_data;

typedef struct __attribute__((packed)) s_target_message
{
	long	team_id;
	t_coord	coord;
	size_t	target_team_id;
}	t_target_message;

typedef void	(*t_shared_data_operation)(t_shared_data *shared_data,
					t_player_data *player_data, int msq_id);

extern bool		g_running;

void			clean_shared_resources(t_shared_resources_ids *ids);
void			with_data(int sem_id, int msq_id, t_shared_data *shared_data,
					t_player_data *player_data,
					t_shared_data_operation op);

size_t			game_loop(size_t team_id, t_shared_data *shared_data,
					const t_shared_resources_ids *shared_resources_ids);
void			player_turn(t_shared_data *shared_data,
					t_player_data *player_data, int msq_id);

#endif

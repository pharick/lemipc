/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:49:23 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/03 21:21:18 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_player.h"

bool	g_running;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_running = false;
}

static void	player_start(t_shared_data *shared_data, t_player_data *player_data)
{
	shared_data->nb_players++;
	player_data->coord.x = rand() % MAP_WIDTH;
	player_data->coord.y = rand() % MAP_HEIGHT;
	while (shared_data->map[player_data->coord.y][player_data->coord.x] != 0)
	{
		player_data->coord.x = rand() % MAP_WIDTH;
		player_data->coord.y = rand() % MAP_HEIGHT;
	}
	shared_data->map[player_data->coord.y][player_data->coord.x]
		= player_data->team_id;
	ft_printf("Player joined team %zu at (%zu, %zu)\n",
		player_data->team_id, player_data->coord.x, player_data->coord.y);
}

static void	player_finish(t_shared_data *shared_data,
	t_player_data *player_data)
{
	shared_data->nb_players--;
	shared_data->map[player_data->coord.y][player_data->coord.x] = 0;
	ft_printf("Player left the game");
}

size_t	game_loop(size_t team_id, t_shared_data *shared_data,
	const t_shared_resources_ids *shared_resources_ids)
{
	t_player_data	*player_data;

	srand(time(NULL));
	signal(SIGINT, handle_sigint);
	player_data = (t_player_data *)malloc(sizeof(t_player_data));
	if (player_data == NULL)
		return (read_shared_data(shared_resources_ids->sem_id,
				shared_data).nb_players);
	player_data->team_id = team_id;
	with_data(shared_resources_ids->sem_id,
		shared_data, player_data, player_start);
	g_running = true;
	while (g_running)
	{
		with_data(shared_resources_ids->sem_id,
			shared_data, player_data, player_turn);
		usleep(500000);
	}
	with_data(shared_resources_ids->sem_id,
		shared_data, player_data, player_finish);
	free(player_data);
	return (read_shared_data(shared_resources_ids->sem_id,
			shared_data).nb_players);
}

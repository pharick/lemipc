/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:49:23 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 15:05:04 by cbelva           ###   ########.fr       */
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
	shared_data->map[player_data->coord.y][player_data->coord.x] = player_data->team_id;
}

static void	print_turn(t_shared_data *data, t_player_data *player_data)
{
	size_t	i;
	size_t	j;

	(void)player_data;
	printf("Number of players: %zu\n", data->nb_players);
	printf("Team %zu. Coord: (%zu, %zu)\n", player_data->team_id,
		player_data->coord.x, player_data->coord.y);
	i = 0;
	while (i < MAP_HEIGHT)
	{
		printf("|");
		j = 0;
		while (j < MAP_WIDTH)
		{
			if (data->map[i][j] == 0)
				printf("   |");
			else
				printf(" %zu |", data->map[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

static void	player_finish(t_shared_data *shared_data,
	t_player_data *player_data)
{
	shared_data->nb_players--;
	shared_data->map[player_data->coord.y][player_data->coord.x] = 0;
	printf("Player %zu. Coord: (%zu, %zu) left the game\n",
		player_data->team_id, player_data->coord.x, player_data->coord.y);
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
			shared_data, player_data, print_turn);
		sleep(1);
		with_data(shared_resources_ids->sem_id,
			shared_data, player_data, player_turn);
		printf("\033[%dA", MAP_HEIGHT + 2);
	}
	with_data(shared_resources_ids->sem_id,
		shared_data, player_data, player_finish);
	free(player_data);
	return (read_shared_data(shared_resources_ids->sem_id,
			shared_data).nb_players);
}

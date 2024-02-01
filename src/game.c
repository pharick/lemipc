/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:49:23 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/01 23:29:16 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

bool	g_running;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_running = false;
}

static void	player_start(t_shared_data *shared_data, t_player_data *player_data)
{
	shared_data->nb_players++;
	while (shared_data->map[player_data->y][player_data->x] != 0)
	{
		player_data->x = rand() % MAP_WIDTH;
		player_data->y = rand() % MAP_HEIGHT;
	}
	shared_data->map[player_data->y][player_data->x] = player_data->team_id;
}

static void	player_finish(t_shared_data *shared_data, t_player_data *player_data)
{
	shared_data->nb_players--;
	shared_data->map[player_data->y][player_data->x] = 0;
}

static void	print_step(t_shared_data *data, t_player_data *player_data)
{
	size_t	i;
	size_t	j;

	(void)player_data;
	printf("Number of players: %zu\n", data->nb_players);
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
			shared_data, player_data, print_step);
		sleep(1);
		printf("\033[%dA", MAP_HEIGHT + 1);
	}
	with_data(shared_resources_ids->sem_id,
		shared_data, player_data, player_finish);
	return (read_shared_data(shared_resources_ids->sem_id,
			shared_data).nb_players);
}

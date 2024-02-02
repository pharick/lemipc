/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turn.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:40:34 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 12:47:26 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

bool	g_running;

static bool	check_game_over(t_coord coord, const size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	size_t	team_id;
	size_t	i;
	size_t	j;
	size_t	enemy_count;

	team_id = map[coord.y][coord.x];
	enemy_count = 0;
	i = coord.y - 1;
	while (i <= coord.y + 1)
	{
		j = coord.x - 1;
		while (j <= coord.x + 1)
		{
			if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH && map[i][j] != 0 && map[i][j] != team_id)
				if (++enemy_count >= 2)
					return (true);
			j++;
		}
		i++;
	}
	return (false);
}

static t_coord	*find_nearest_enemy(const t_coord *coord, const size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	t_coord	*nearest_enemy;
	size_t	min_distance;
	size_t	distance;
	size_t	i;
	size_t	j;

	min_distance = MAP_WIDTH * MAP_HEIGHT;
	nearest_enemy = NULL;
	i = 0;
	while (i < MAP_HEIGHT)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			if (map[i][j] != 0 && map[i][j] != map[coord->y][coord->x])
			{
				distance = abs((int)i - (int)coord->y) + abs((int)j - (int)coord->x);
				if (distance < min_distance)
				{
					min_distance = distance;
					if (nearest_enemy == NULL)
						nearest_enemy = malloc(sizeof(t_coord));
					nearest_enemy->x = j;
					nearest_enemy->y = i;
				}
			}
			j++;
		}
		i++;
	}
	return (nearest_enemy);
}

static void	move_towards_coord(t_coord *coord, const t_coord *target, size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	int		dx;
	int		dy;
	t_coord	new_coord;

	dx = (int)target->x - (int)coord->x;
	dy = (int)target->y - (int)coord->y;
	if (abs(dx) > abs(dy))
	{
		new_coord.x = coord->x + (dx > 0 ? 1 : -1);
		new_coord.y = coord->y;
	}
	else
	{
		new_coord.x = coord->x;
		new_coord.y = coord->y + (dy > 0 ? 1 : -1);
	}
	if (new_coord.x >= 0 && new_coord.x < MAP_WIDTH && new_coord.y >= 0 && new_coord.y < MAP_HEIGHT
		&& map[new_coord.y][new_coord.x] == 0)
	{
		map[new_coord.y][new_coord.x] = map[coord->y][coord->x];
		map[coord->y][coord->x] = 0;
		*coord = new_coord;	
	}
}

void	player_turn(t_shared_data *shared_data, t_player_data *player_data)
{
	t_coord	*nearest_enemy;

	if (check_game_over(player_data->coord, shared_data->map))
	{
		g_running = false;
		return ;
	}
	nearest_enemy = find_nearest_enemy(&player_data->coord, shared_data->map);
	if (nearest_enemy == NULL)
		return ;
	move_towards_coord(&player_data->coord, nearest_enemy, shared_data->map);
	free(nearest_enemy);
}

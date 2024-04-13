/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turn.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:40:34 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/05 00:40:05 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_player.h"

static bool	check_game_over(t_coord coord,
	const size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	size_t	team_id;
	ssize_t	i;
	ssize_t	j;
	ssize_t	enemy_counts[MAX_TEAMS];

	team_id = map[coord.y][coord.x];
	memset(enemy_counts, 0, sizeof(enemy_counts));
	i = coord.y - 1;
	while (i <= coord.y + 1)
	{
		j = coord.x - 1;
		while (j <= coord.x + 1)
		{
			if (i >= 0 && i < MAP_HEIGHT && j >= 0
				&& j < MAP_WIDTH && map[i][j] != 0
				&& map[i][j] != team_id)
				if (++enemy_counts[map[i][j]] >= 2)
					return (true);
			j++;
		}
		i++;
	}
	return (false);
}

static t_coord	*find_nearest_enemy(const t_coord *coord,
	const size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	t_coord	*nearest_enemy;
	size_t	min_distance;
	size_t	distance;
	ssize_t	i;
	ssize_t	j;

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
				distance = abs((int)i - (int)coord->y)
					+ abs((int)j - (int)coord->x);
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

t_coord	get_around_coords(const t_coord *coord, t_coord target)
{
	t_coord new_coord;

	if (target.x == coord->x)
	{
		new_coord.y = coord->y;
		if (target.y > coord->y)
			new_coord.x = coord->x + 1;
		else
			new_coord.x = coord->x - 1;
	}
	else
	{
		new_coord.x = coord->x;
		if (target.x > coord->x)
			new_coord.y = coord->y + 1;
		else
			new_coord.y = coord->y - 1;
	}
	return (new_coord);
}

bool	move_towards_coord(t_coord *coord, const t_coord *target,
	size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	int		dx;
	int		dy;
	t_coord	new_coord;

	dx = (int)target->x - (int)coord->x;
	dy = (int)target->y - (int)coord->y;
	if (abs(dx) > abs(dy))
	{
		if (dx > 0)
			new_coord.x = coord->x + 1;
		else
			new_coord.x = coord->x - 1;
		new_coord.y = coord->y;
	}
	else
	{
		new_coord.x = coord->x;
		if (dy > 0)
			new_coord.y = coord->y + 1;
		else
			new_coord.y = coord->y - 1;
	}
	if (new_coord.x < 0 || new_coord.x >= MAP_WIDTH || new_coord.y < 0 || new_coord.y >= MAP_HEIGHT)
		return (false);
	if (map[new_coord.y][new_coord.x] != 0)
	{
		ft_printf("Stuck. Can't move to (%zu, %zu)\n", new_coord.x, new_coord.y);
		new_coord = get_around_coords(coord, new_coord);
		if (new_coord.x < 0 || new_coord.x >= MAP_WIDTH
			|| new_coord.y < 0 || new_coord.y >= MAP_HEIGHT
			|| map[new_coord.y][new_coord.x] != 0)
			return (false);
		ft_printf("Going around to (%zu, %zu)\n", new_coord.x, new_coord.y);
	}
	ft_printf("Moving from (%zu, %zu) to (%zu, %zu)\n",
		coord->x, coord->y, new_coord.x, new_coord.y);
	map[new_coord.y][new_coord.x] = map[coord->y][coord->x];
	map[coord->y][coord->x] = 0;
	*coord = new_coord;
	return (true);
}

void	recieve_target_from_queue(int msq_id, size_t team_id, t_coord **coord, size_t *target_team_id)
{
	t_target_message	message;
	ssize_t				ret;

	*coord = NULL;
	ret = msgrcv(msq_id, &message, sizeof(t_target_message) - sizeof(long), team_id, IPC_NOWAIT);
	if (ret == -1)
	{
		if (errno != ENOMSG)
			ft_printf("Error receiving message: %s\n", strerror(errno));
		return ;
	}
	*coord = malloc(sizeof(t_coord));
	if (*coord == NULL)
		return ;
	**coord = message.coord;
	*target_team_id = message.target_team_id;
}

void	send_target_to_queue(int msq_id, size_t team_id, t_coord coord, size_t target_team_id)
{
	t_target_message	message;

	message.team_id = team_id;
	message.coord = coord;
	message.target_team_id = target_team_id;
	if (msgsnd(msq_id, &message, sizeof(t_target_message) - sizeof(long), 0) == -1)
		ft_printf("Error sending message: %s\n", strerror(errno));
}

void	player_turn(t_shared_data *shared_data, t_player_data *player_data, int msq_id)
{
	t_coord	*target;
	size_t	target_team_id;

	if (check_game_over(player_data->coord, shared_data->map))
	{
		g_running = false;
		ft_printf("Player was caught\n");
		return ;
	}
	recieve_target_from_queue(msq_id, player_data->team_id, &target, &target_team_id);
	if (target != NULL
		&& target->x >= 0 && target->x < MAP_WIDTH
		&& target->y >= 0 && target->y < MAP_HEIGHT
		&& shared_data->map[target->y][target->x] == target_team_id)
	{
		ft_printf("Recieved target (%zu, %zu) from message queue\n", target->x, target->y);
		move_towards_coord(&player_data->coord, target, shared_data->map);
		send_target_to_queue(msq_id, player_data->team_id, player_data->coord, target_team_id);
		free(target);
		return ;
	}
	if (target != NULL)
		free(target);
	target = find_nearest_enemy(&player_data->coord, shared_data->map);
	if (target == NULL)
		return ;
	move_towards_coord(&player_data->coord, target, shared_data->map);
	target_team_id = shared_data->map[target->y][target->x];
	ft_printf("Found target by team %zu at (%zu, %zu)\n", target_team_id, target->x, target->y);
	send_target_to_queue(msq_id, player_data->team_id, *target, target_team_id);
	free(target);
}

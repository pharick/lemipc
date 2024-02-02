/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:26:48 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 16:11:13 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_player.h"

static size_t	parse_team_id(const char *str)
{
	size_t	team_id;
	char	*endptr;

	if (str == NULL || str[0] == '-')
		return (0);
	errno = 0;
	team_id = strtoul(str, &endptr, 10);
	if (errno != 0 || *endptr != '\0')
		return (0);
	return (team_id);
}

int	main(int argc, char **argv)
{
	t_shared_resources_ids	*shared_resources_ids;
	t_shared_data			*shared_data;
	size_t					team_id;
	size_t					nb_players_left;

	if (argc < 2)
	{
		printf("Usage: %s <team_id>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	team_id = parse_team_id(argv[1]);
	if (team_id == 0)
	{
		fprintf(stderr, "Invalid team id. Should be a positive integer.\n");
		return (EXIT_FAILURE);
	}
	shared_resources_ids = get_shared_resources(true);
	if (shared_resources_ids == NULL)
	{
		fprintf(stderr, "Failed to get shared resources: %s\n",
			strerror(errno));
		return (EXIT_FAILURE);
	}
	shared_data = shmat(shared_resources_ids->shm_id, NULL, 0);
	if (shared_data == (void *)-1)
	{
		fprintf(stderr, "Failed to attach shared memory: %s\n",
			strerror(errno));
		clean_shared_resources(shared_resources_ids);
		return (EXIT_FAILURE);
	}
	nb_players_left = game_loop(team_id, shared_data, shared_resources_ids);
	shmdt(shared_data);
	if (nb_players_left <= 0)
		clean_shared_resources(shared_resources_ids);
	return (EXIT_SUCCESS);
}

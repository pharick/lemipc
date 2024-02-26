/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:26:48 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/04 15:49:04 by cbelva           ###   ########.fr       */
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

int	return_failure(const char *msg, bool print_errno)
{	
	if (msg != NULL)
	{
		fprintf(stderr, "%s", msg);
		if (print_errno)
			fprintf(stderr, ": %s", strerror(errno));
		fprintf(stderr, "\n");
	}
	return (EXIT_FAILURE);
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
	if (team_id == 0 || team_id > MAX_TEAMS)
		return (return_failure("Team id should be between 1 and 10", false));
	shared_resources_ids = get_shared_resources(true);
	if (shared_resources_ids == NULL)
		return (return_failure("Failed to get shared resources", true));
	shared_data = shmat(shared_resources_ids->shm_id, NULL, 0);
	if (shared_data == (void *)-1)
	{
		clean_shared_resources(shared_resources_ids);
		return (return_failure("Failed to attach to shared memory", true));
	}
	nb_players_left = game_loop(team_id, shared_data, shared_resources_ids);
	shmdt(shared_data);
	if (nb_players_left <= 0)
		clean_shared_resources(shared_resources_ids);
	return (EXIT_SUCCESS);
}

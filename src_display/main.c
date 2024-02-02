/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:39:11 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 18:13:13 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_display.h"

static t_app	*init_app(void)
{
	t_app	*app;

	app = malloc(sizeof(t_app));
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return (NULL);
	}
	app->window = SDL_CreateWindow("lemipc", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (app->window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return (NULL);
	}
	app->renderer = SDL_CreateRenderer(app->window, -1,
			SDL_RENDERER_ACCELERATED);
	if (app->renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(app->window);
		SDL_Quit();
		return (NULL);
	}
	return (app);
}

static void	destroy_app(t_app *app)
{
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	SDL_Quit();
	free(app);
}

static t_color	get_team_color(size_t team_id)
{
	t_color	color;
	double	hue;
	double	rad;

	hue = ((team_id * 157) % 360);
	rad = hue * M_PI / 180.0;
	color.r = sin(rad) * 127.5 + 127.5;
	color.g = sin(rad + 2.0 * M_PI / 3.0) * 127.5 + 127.5;
	color.b = sin(rad + 4.0 * M_PI / 3.0) * 127.5 + 127.5;
	color.a = 255;
	return (color);
}

void	render_map(SDL_Renderer *renderer,
	const size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	size_t		cell_width;
	size_t		cell_height;
	size_t		i;
	size_t		j;
	SDL_Rect	rect;
	t_color		color;

	cell_width = WINDOW_WIDTH / MAP_WIDTH;
	cell_height = WINDOW_HEIGHT / MAP_HEIGHT;
	i = 0;
	while (i < MAP_HEIGHT)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			rect.x = j * cell_width;
			rect.y = i * cell_height;
			rect.w = cell_width;
			rect.h = cell_height;
			if (map[i][j] != 0)
			{
				color = get_team_color(map[i][j]);
				printf("color for %zu: %d %d %d\n", map[i][j],
					color.r, color.g, color.b);
				SDL_SetRenderDrawColor(renderer,
					color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(renderer, &rect);
			}
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(renderer, &rect);
			j++;
		}
		i++;
	}
}

int	main(void)
{
	t_app					*app;
	SDL_Event				event;
	t_shared_resources_ids	*shared_resources_ids;
	t_shared_data			*shared_data;

	shared_resources_ids = get_shared_resources(false);
	if (shared_resources_ids == NULL)
	{
		fprintf(stderr, "Failed to get shared resources: %s\n",
			strerror(errno));
		return (EXIT_FAILURE);
	}
	shared_data = shmat(shared_resources_ids->shm_id, NULL, 0);
	if (shared_data == (void *)-1)
	{
		fprintf(stderr, "shmat: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	app = init_app();
	if (app == NULL)
		return (EXIT_FAILURE);
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break ;
		}
		SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
		SDL_RenderClear(app->renderer);
		render_map(app->renderer, shared_data->map);
		SDL_RenderPresent(app->renderer);
	}
	destroy_app(app);
	shmdt(shared_data);
	return (EXIT_SUCCESS);
}

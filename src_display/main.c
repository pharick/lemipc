/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:39:11 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/03 18:28:58 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc_display.h"

static t_app	*init_app(void)
{
	t_app	*app;

	app = malloc(sizeof(t_app));
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		ft_printf("SDL_Init: %s\n", SDL_GetError());
		return (NULL);
	}
	app->window = SDL_CreateWindow("lemipc", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (app->window == NULL)
	{
		ft_printf("SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return (NULL);
	}
	app->renderer = SDL_CreateRenderer(app->window, -1,
			SDL_RENDERER_ACCELERATED);
	if (app->renderer == NULL)
	{
		ft_printf("SDL_CreateRenderer: %s\n", SDL_GetError());
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

void	render_map(t_app *app, const size_t map[MAP_HEIGHT][MAP_WIDTH])
{
	int			window_width;
	int			window_height;
	size_t		cell_width;
	size_t		cell_height;
	size_t		i;
	size_t		j;
	SDL_Rect	rect;
	t_color		color;

	SDL_GetWindowSize(app->window, &window_width, &window_height);
	cell_width = window_width / MAP_WIDTH;
	cell_height = window_height / MAP_HEIGHT;
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
				SDL_SetRenderDrawColor(app->renderer,
					color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(app->renderer, &rect);
			}
			SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(app->renderer, &rect);
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
	while (shared_resources_ids == NULL)
	{
		ft_printf("Failed to get shared resources. Perhaps there is no players running yet.\n");
		sleep(1);
		shared_resources_ids = get_shared_resources(false);
	}
	shared_data = shmat(shared_resources_ids->shm_id, NULL, 0);
	if (shared_data == (void *)-1)
	{
		ft_printf("shmat: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	app = init_app();
	if (app == NULL) {
		shmdt(shared_data);
		return (EXIT_FAILURE);
	}
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break ;
		}
		SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
		SDL_RenderClear(app->renderer);
		render_map(app, shared_data->map);
		SDL_RenderPresent(app->renderer);
	}
	destroy_app(app);
	shmdt(shared_data);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:39:11 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 15:57:12 by cbelva           ###   ########.fr       */
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
	app->window = SDL_CreateWindow("lemipc", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (app->window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return (NULL);
	}
	app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
	if (app->renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(app->window);
		SDL_Quit();
		return (NULL);
	}
	return (app);
}

void destroy_app(t_app *app)
{
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	SDL_Quit();
	free(app);
}

void render_map(SDL_Renderer *renderer)
{
	size_t		cell_width;
	size_t		cell_height;
	size_t		i;
	size_t		j;
	SDL_Rect	rect;

	cell_width = WINDOW_WIDTH / MAP_WIDTH;
	cell_height = WINDOW_HEIGHT / MAP_HEIGHT;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
			SDL_RenderDrawRect(renderer, &rect);
			j++;
		}
		i++;
	}
}

int	main(void)
{
	t_app		*app;
	SDL_Event	event;

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
		render_map(app->renderer);
		SDL_RenderPresent(app->renderer);
	}
	destroy_app(app);
	return (EXIT_SUCCESS);
}

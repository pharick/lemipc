/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc_display.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 15:06:25 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/05 01:25:12 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_DISPLAY_H
# define LEMIPC_DISPLAY_H

# include <stdbool.h>
# include <math.h>

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>

# include "lemipc_common.h"

# define WINDOW_WIDTH 600
# define WINDOW_HEIGHT 600

typedef struct s_app
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	TTF_Font		*font;
}	t_app;

typedef struct s_color
{
	Uint8	r;
	Uint8	g;
	Uint8	b;
	Uint8	a;
}	t_color;

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc_display.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 15:06:25 by cbelva            #+#    #+#             */
/*   Updated: 2024/02/02 15:50:13 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_DISPLAY_H
# define LEMIPC_DISPLAY_H

#include <stdbool.h>

#include <SDL2/SDL.h>

# include "lemipc_common.h"

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600

typedef struct s_app
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
}	t_app;

#endif
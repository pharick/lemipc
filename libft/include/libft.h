/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:49:41 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/05 01:34:37 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stddef.h>
# include <stdlib.h>
# include "ft_printf.h"

void	ft_putchar(char c);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
int		ft_nbrlen(int n);
size_t	ft_pow(size_t n, unsigned int p);
int		ft_strcmp(const char *str1, const char *str2);
char	*ft_itoa(int n);

#endif

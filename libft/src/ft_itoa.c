/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 01:33:50 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/05 01:34:12 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_num_len(int n)
{
	int	len;

	len = 1;
	while (n /= 10)
		len++;
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		sign;

	sign = n < 0;
	len = get_num_len(n) + sign;
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	str[len] = '\0';
	if (sign)
		str[0] = '-';
	while (len-- > sign)
	{
		str[len] = n % 10 * (sign ? -1 : 1) + '0';
		n /= 10;
	}
	return (str);
}

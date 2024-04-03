/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_specifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbelva <cbelva@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:45:54 by cbelva            #+#    #+#             */
/*   Updated: 2024/04/03 17:53:44 by cbelva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_print_string(t_specifier specifier, va_list *ap)
{
	char	*str;
	int		len;

	str = va_arg(*ap, char *);
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	if (specifier.precision >= 0 && specifier.precision < len)
		len = specifier.precision;
	while (len-- > 0)
	{
		ft_putchar(*str++);
	}
	return (len);
}

static int	ft_print_integer(t_specifier specifier, va_list *ap)
{
	int	n;
	int	len;

	n = va_arg(*ap, int);
	len = ft_nbrlen(n);
	if (specifier.precision >= 0 && specifier.precision > len)
		len = specifier.precision;
	if (n < 0)
		len++;
	while (len-- > 0)
	{
		ft_putchar('0' + n / (int)ft_pow(10, len));
		n %= (int)ft_pow(10, len);
	}
	return (len);
}

static int	ft_print_unsigned(t_specifier specifier, va_list *ap)
{
	unsigned int	n;
	int				len;

	n = va_arg(*ap, unsigned int);
	len = ft_nbrlen(n);
	if (specifier.precision >= 0 && specifier.precision > len)
		len = specifier.precision;
	while (len-- > 0)
	{
		ft_putchar('0' + n / (int)ft_pow(10, len));
		n %= (int)ft_pow(10, len);
	}
	return (len);
}

static int	ft_print_unsigned_long_long(t_specifier specifier, va_list *ap)
{
	unsigned long long	n;
	int					len;

	n = va_arg(*ap, unsigned long long);
	len = ft_nbrlen(n);
	if (specifier.precision >= 0 && specifier.precision > len)
		len = specifier.precision;
	while (len-- > 0)
	{
		ft_putchar('0' + n / (int)ft_pow(10, len));
		n %= (int)ft_pow(10, len);
	}
	return (len);
}

static int	ft_print_hex(t_specifier specifier, va_list *ap)
{
	unsigned int	n;
	int				len;
	char			*base;

	n = va_arg(*ap, unsigned int);
	if (specifier.type == 'x')
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	len = ft_nbrlen(n);
	if (specifier.precision >= 0 && specifier.precision > len)
		len = specifier.precision;
	while (len-- > 0)
	{
		ft_putchar(base[n / (int)ft_pow(16, len)]);
		n %= (int)ft_pow(16, len);
	}
	return (len);
}

static int	ft_print_pointer(t_specifier specifier, va_list *ap)
{
	unsigned long long	n;
	int					len;
	char				*base;

	n = va_arg(*ap, unsigned long long);
	base = "0123456789abcdef";
	len = ft_nbrlen(n);
	if (specifier.precision >= 0 && specifier.precision > len)
		len = specifier.precision;
	while (len-- > 0)
	{
		ft_putchar(base[n / (int)ft_pow(16, len)]);
		n %= (int)ft_pow(16, len);
	}
	return (len);
}

int	ft_print_specifier(t_specifier specifier, va_list *ap)
{
	if (specifier.type == 's')
		return (ft_print_string(specifier, ap));
	else if (specifier.type == 'd' || specifier.type == 'i')
		return (ft_print_integer(specifier, ap));
	else if (specifier.type == 'u') {
		if (!ft_strcmp(specifier.length, "ll") || !ft_strcmp(specifier.length, "z"))
			return (ft_print_unsigned_long_long(specifier, ap));
		return (ft_print_unsigned(specifier, ap));
	} else if (specifier.type == 'x' || specifier.type == 'X')
		return (ft_print_hex(specifier, ap));
	else if (specifier.type == 'p')
		return (ft_print_pointer(specifier, ap));
	else if (specifier.type == 'c')
	{
		ft_putchar(va_arg(*ap, int));
		return (1);
	}
	else if (specifier.type == '%')
	{
		ft_putchar('%');
		return (1);
	}
	return (0);
}

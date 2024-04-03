#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include <stddef.h>
# include <stdio.h>
# include <math.h>

# define MINUS_FLAG			0b00000001
# define PLUS_FLAG			0b00000010
# define SPACE_FLAG			0b00000100
# define ZERO_FLAG			0b00001000
# define APOSTROPHE_FLAG	0b00010000
# define HASH_FLAG			0b00100000

typedef struct s_specifier
{
	uint8_t	flags;
	int		width;
	int		precision;
	char	*length;
	char	type;
}	t_specifier;

int			ft_printf(const char *format, ...);
t_specifier	ft_parse_specifier(const char **format, va_list *ap);
int			ft_print_specifier(t_specifier specifier, va_list *ap);
void		ft_print_specifier_info(t_specifier specifier);

#endif

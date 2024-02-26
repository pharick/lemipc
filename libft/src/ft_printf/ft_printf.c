#include "libft.h"

int	ft_printf(const char *format, ...)
{
	va_list		ap;
	int			count;
	t_specifier	specifier;

	va_start(ap, format);

	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			specifier = ft_parse_specifier(&format, &ap);
			count += ft_print_specifier(specifier, &ap);
		}
		else
		{
			ft_putchar(*format);
			count++;
			format++;
		}
	}

	va_end(ap);
	return (count);
}

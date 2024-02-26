#include "libft.h"

static uint8_t	ft_parse_flags(const char **format)
{
	uint8_t	flags;

	flags = 0;
	while (ft_strchr("-+ 0'#", **format))
	{
		if (**format == '-')
			flags |= MINUS_FLAG;
		else if (**format == '+')
			flags |= PLUS_FLAG;
		else if (**format == ' ')
			flags |= SPACE_FLAG;
		else if (**format == '0')
			flags |= ZERO_FLAG;
		else if (**format == '\'')
			flags |= APOSTROPHE_FLAG;
		else if (**format == '#')
			flags |= HASH_FLAG;
		(*format)++;
	}
	return (flags);
}

static int	ft_parse_width(const char **format, va_list *ap)
{
	int	width;

	width = -1;
	if (ft_isdigit(**format))
	{
		width = ft_atoi(*format);
		while (ft_isdigit(**format))
			(*format)++;
	}
	else if (**format == '*')
	{
		width = va_arg(*ap, int);
		(*format)++;
	}
	return (width);
}

static int	ft_parse_precision(const char **format, va_list *ap)
{
	int	precision;

	precision = -1;
	if (**format == '.')
	{
		(*format)++;
		if (ft_isdigit(**format))
		{
			precision = ft_atoi(*format);
			while (ft_isdigit(**format))
				(*format)++;
		}
		else if (**format == '*')
		{
			precision = va_arg(*ap, int);
			(*format)++;
		}
	}
	return (precision);
}

static char	*ft_parse_length(const char **format)
{
	char	*length;

	length = NULL;
	if (ft_strchr("hlLzj", **format))
	{
		if (**format == 'h' && *(*format + 1) == 'h')
			length = "hh";
		else if (**format == 'h')
			length = "h";
		else if (**format == 'l' && *(*format + 1) == 'l')
			length = "ll";
		else if (**format == 'l')
			length = "l";
		else if (**format == 'L')
			length = "L";
		else if (**format == 'z')
			length = "z";
		else if (**format == 'j')
			length = "j";
		if (length)
			(*format) += ft_strlen(length) - 1;
	}
	return (length);
}

static char	ft_parse_type(const char **format)
{
	char	type;

	type = 0;
	if (ft_strchr("cspdiuxX%", **format))
	{
		type = **format;
		(*format)++;
	}
	return (type);
}

t_specifier	ft_parse_specifier(const char **format, va_list *ap)
{
	t_specifier	specifier;

	specifier.flags = ft_parse_flags(format);
	specifier.width = ft_parse_width(format, ap);
	specifier.precision = ft_parse_precision(format, ap);
	specifier.length = ft_parse_length(format);
	specifier.type = ft_parse_type(format);
	return (specifier);
}

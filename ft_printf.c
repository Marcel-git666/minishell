/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:27:51 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/05 15:07:37 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_format(char conversion, va_list *ap)
{
	int		count;

	count = 0;
	if (conversion == 'c')
		count += print_char(va_arg(*ap, int));
	else if (conversion == 's')
		count += print_string(va_arg(*ap, char *));
	else if (conversion == 'd' || conversion == 'i')
		count += print_digit((long)va_arg(*ap, int), 10, "0123456789");
	else if (conversion == 'l')
		count += print_digit((long)va_arg(*ap, long int), 10, "0123456789");
	else if (conversion == 'u')
		count += print_digit(va_arg(*ap, unsigned int), 10, "0123456789");
	else if (conversion == 'x')
		count += print_digit((long)va_arg(*ap, unsigned int),
				16, "0123456789abcdef");
	else if (conversion == 'X')
		count += print_digit((long)va_arg(*ap, unsigned int),
				16, "0123456789ABCDEF");
	else if (conversion == 'p')
		count += print_pointer(va_arg(*ap, void *));
	else
		count += write(1, &conversion, 1);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, format);
	count = 0;
	while (*format != '\0')
	{
		if (*format == '%')
			count += print_format(*++format, &ap);
		else
			count += write(1, format, 1);
		++format;
	}
	va_end(ap);
	return (count);
}

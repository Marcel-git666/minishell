/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:27:51 by mmravec           #+#    #+#             */
/*   Updated: 2024/10/04 18:56:00 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

// #include <limits.h>
// #include <stdio.h>

// int main(void)
// {
//     int     count;
//     int     x = 42;
//     int     *ptr = &x;
// 	char	*name = "Ka\b\atka";

//     count = ft_printf("Hello\n\t\v %s!\n", name);
// 	ft_printf("Chars written are: %d\n", count);
// 	count = printf("Hello\n\t\v %s!\n", name);
//     ft_printf("Chars written are: %d\n", count);
//     count = ft_printf("%X\n", 42);
//     ft_printf("Chars written are: %d\n", count);
//     count = ft_printf("%u\n", -10);  // Be careful with unsigned numbers
//     ft_printf("Chars written are: %d\n", count);
//     // Testing %p conversion
//     count = ft_printf("Pointer to x: %p\n", (void*)ptr);
//     ft_printf("Chars written are: %d\n", count);
//     // Testing NULL pointer
//     count = ft_printf("NULL pointer: %p\n", NULL);
//     ft_printf("Chars written are: %d\n", count);
// 	count = ft_printf(" %c \n", '1');
// 	ft_printf("Chars written are: %d\n", count);
// 	count = ft_printf(" %c %c %c \n", '1', 0, '3');
// 	ft_printf("Chars written are: %d\n", count);
// 	ft_printf(" %p %p \n", (void*)(unsigned long int)LONG_MIN,
// 		(void*)(unsigned long int)LONG_MAX);
// 	ft_printf(" %p %p \n", (void*)(unsigned long int)ULONG_MAX,
// 		(void*)(unsigned long int)(-ULONG_MAX));
//     return 0;
// }

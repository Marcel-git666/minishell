/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:55:45 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/26 15:11:23 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(int c)
{
	return (write(1, &c, 1));
}

int	print_string(char *s)
{
	int		count;

	count = 0;
	if (s == NULL)
		return (write(1, "(null)", 6));
	while (*s)
	{
		count += write(1, s, 1);
		s++;
	}
	return (count);
}

int	print_digit(long n, int base, char *symbols)
{
	int		count;

	count = 0;
	if (n < 0)
	{
		write(1, "-", 1);
		return (print_digit(-n, base, symbols) + 1);
	}
	else if (n < base)
		return (print_char(symbols[n]));
	else
	{
		count = print_digit(n / base, base, symbols);
		return (count + print_digit(n % base, base, symbols));
	}
}

int	print_unsigned_digit(unsigned long long n, int base, char *symbols)
{
	int		count;
	char	buffer[65];
	int		i;

	count = 0;
	i = 0;
	if (n == 0)
		return (print_char('0'));
	while (n > 0)
	{
		buffer[i++] = symbols[n % base];
		n /= base;
	}
	while (--i >= 0)
		count += print_char(buffer[i]);
	return (count);
}

int	print_pointer(void *p)
{
	int					count;
	unsigned long int	address;

	count = 0;
	if (p == NULL)
		return (write(1, "(nil)", 5));
	address = (unsigned long int)p;
	count += write(1, "0x", 2);
	count += print_unsigned_digit(address, 16, "0123456789abcdef");
	return (count);
}

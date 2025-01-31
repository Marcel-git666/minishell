/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:28:20 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/26 12:23:21 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

int	ft_printf(const char *format, ...);
int	print_format(char conversion, va_list *ap);
int	print_char(int c);
int	print_string(char *s);
int	print_digit(long n, int base, char *symbols);
int	print_unsigned_digit(unsigned long long n, int base, char *symbols);
int	print_pointer(void *p);

#endif

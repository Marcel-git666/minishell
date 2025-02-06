/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:11:56 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/17 10:37:41 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_rev_char_tab(char *tab, int size)
{
	char	temp;
	int		start;
	int		end;

	start = 0;
	end = size - 1;
	while (start < end)
	{
		temp = tab[start];
		tab[start] = tab[end];
		tab[end] = temp;
		start++;
		end--;
	}
}

static int	is_neg(int n)
{
	if (n < 0)
		return (1);
	else
		return (0);
}

static int	count_digits(int n)
{
	int		count;

	if (n == -2147483648)
		return (11);
	count = 0;
	if (n < 0)
	{
		count++;
		n = -n;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		i;
	int		temp;
	int		is_negative;

	is_negative = is_neg(n);
	i = 0;
	result = (char *)malloc((count_digits(n) + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (n == 0)
		result[i++] = '0';
	while (n != 0)
	{
		temp = n % 10;
		if (is_negative)
			temp *= (-1);
		result[i++] = temp + '0';
		n /= 10;
	}
	if (is_negative)
		result[i++] = '-';
	result[i] = '\0';
	ft_rev_char_tab(result, i);
	return (result);
}

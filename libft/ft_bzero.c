/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:58:53 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/16 11:32:55 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*origin;
	size_t			i;

	i = 0;
	origin = (unsigned char *)s;
	while (i < n)
	{
		origin[i] = 0;
		i++;
	}
}

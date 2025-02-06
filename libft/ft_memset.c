/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 18:25:52 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/16 13:11:31 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*origin;
	size_t			i;

	i = 0;
	origin = (unsigned char *)b;
	while (i < len)
	{
		origin[i] = c;
		i++;
	}
	return (b);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:09:06 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/16 21:26:16 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, char const *charset)
{
	while (*charset)
		if (c == *charset++)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	end;
	size_t	length;

	if (!s1 || !set)
		return (NULL);
	while (is_in_set(*s1, set))
		s1++;
	if (*s1 == '\0')
		return (ft_strdup(""));
	end = ft_strlen(s1) - 1;
	while (end > 0 && is_in_set(s1[end], set))
		end--;
	length = end + 1;
	result = malloc((length + 1) * sizeof(char));
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, length);
	result[length] = '\0';
	return (result);
}

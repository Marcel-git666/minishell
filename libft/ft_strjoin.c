/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:12:46 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/16 13:26:55 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	result_length;

	if (!s1 || !s2)
		return (NULL);
	if (*s1 == '\0')
		return (ft_strdup(s2));
	if (*s2 == '\0')
		return (ft_strdup(s1));
	result_length = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = (char *)malloc(result_length * sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, result_length);
	ft_strlcpy(result + ft_strlen(s1), s2, result_length - ft_strlen(s1));
	return (result);
}

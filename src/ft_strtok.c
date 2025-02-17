/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 20:34:02 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/08 20:58:10 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* find index of first s1[i] that matches any s2[]. */
static size_t	ft_strcspn(const char *s1, const char *s2)
{
	const char	*sc1;
	const char	*sc2;

	sc1 = s1;
	while (*sc1 != '\0')
	{
		sc2 = s2;
		while (*sc2 != '\0')
		{
			if (*sc1 == *sc2)
				return (sc1 - s1);
			sc2++;
		}
		sc1++;
	}
	return (sc1 - s1);
}

/* find index of first s1[i] that matches no s2[]. */
static size_t	ft_strspn(const char *s1, const char *s2)
{
	const char	*sc1;
	const char	*sc2;

	sc1 = s1;
	while (*sc1 != '\0')
	{
		sc2 = s2;
		while (1)
		{
			if (*sc2 == '\0')
				return (sc1 - s1);
			else
				if (*sc1 == *sc2)
					break ;
			sc2++;
		}
		sc1++;
	}
	return (sc1 - s1);
}

/* find next token in s1[] delimited by s2[]. */
char	*ft_strtok(char *s1, const char *s2)
{
	char		*sbegin;
	char		*send;
	static char	*ssave = "";

	if (s1)
		sbegin = s1;
	else
		sbegin = ssave;
	sbegin += ft_strspn(sbegin, s2);
	if (*sbegin == '\0')
	{
		ssave = "";
		return (NULL);
	}
	send = sbegin + ft_strcspn(sbegin, s2);
	if (*send != '\0')
		*send++ = '\0';
	ssave = send;
	return (sbegin);
}

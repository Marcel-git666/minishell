/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:26:55 by mmravec           #+#    #+#             */
/*   Updated: 2024/10/09 19:26:46 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str, char c)
{
	int		words;
	int		is_inside_word;

	words = 0;
	is_inside_word = 0;
	while (*str)
	{
		if (*str == c)
		{
			is_inside_word = 0;
		}
		else
		{
			if (!is_inside_word)
			{
				words++;
				is_inside_word = 1;
			}
		}
		str++;
	}
	return (words);
}

static void	free_allocated_memory(char **result, int words)
{
	int		i;

	i = 0;
	while (i < words)
		free(result[i++]);
	free(result);
}

static int	handle_word(char **result, char *current_word, int word_length,
	int *words)
{
	char	*word;

	word = (char *)malloc((word_length + 1) * sizeof(char));
	if (!word)
	{
		free_allocated_memory(result, *words);
		return (0);
	}
	ft_strlcpy(word, current_word, word_length + 1);
	result[*words] = word;
	(*words)++;
	return (1);
}

static int	extract_words(char **result, char const **s, char c, int *words)
{
	char	*current_word;
	int		word_length;

	current_word = NULL;
	word_length = 0;
	while (**s)
	{
		if (**s == c)
		{
			if (word_length > 0
				&& !handle_word(result, current_word, word_length, words))
				return (0);
			word_length = 0;
		}
		else
			if (word_length++ == 0)
				current_word = (char *)*s;
		(*s)++;
	}
	if (word_length > 0
		&& !handle_word(result, current_word, word_length, words))
		return (0);
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		words;

	words = 0;
	if (!s)
		return (NULL);
	result = (char **) malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!extract_words(result, &s, c, &words))
		return (NULL);
	result[words] = NULL;
	return (result);
}

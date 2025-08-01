/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:41:50 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 14:44:29 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Handles quote state transitions during word extraction
 * Updates quote tracking variables based on current character
 */
static void	handle_quote_processing(char c, int *in_quotes, char *quote_type)
{
	if (c == '\'' || c == '"')
	{
		if (*in_quotes == 0)
		{
			*in_quotes = 1;
			*quote_type = c;
		}
		else if (c == *quote_type)
		{
			*in_quotes = 0;
		}
	}
}

/*
 * Checks if current character represents a word boundary
 * Returns 1 if at word boundary, 0 to continue word extraction
 */
static int	is_word_boundary(char c, int in_quotes)
{
	if (in_quotes == 0 && (ft_isspace(c) || c == '|' || c == '<' || c == '>'))
		return (1);
	return (0);
}

/*
 * Validates that all quotes are properly closed
 * Returns 1 if quotes are valid, 0 if unclosed quotes found
 */
static int	validate_quote_closure(int in_quotes)
{
	if (in_quotes != 0)
	{
		error_message("syntax error: missing closing quote");
		return (0);
	}
	return (1);
}

/*
 * Extracts complete word from input, stopping at word boundaries
 * Handles quotes properly and validates quote closure
 * Returns allocated word string or NULL on error
 */
char	*extract_greedy_word(const char *input, size_t *index)
{
	size_t	start;
	int		in_quotes;
	char	quote_type;
	char	c;

	start = *index;
	in_quotes = 0;
	quote_type = 0;
	while (input[*index] != '\0')
	{
		c = input[*index];
		handle_quote_processing(c, &in_quotes, &quote_type);
		if (is_word_boundary(c, in_quotes))
			break ;
		(*index)++;
	}
	if (!validate_quote_closure(in_quotes))
		return (NULL);
	return (ft_strndup(input + start, *index - start));
}

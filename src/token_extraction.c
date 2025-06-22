/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:46:37 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/22 12:58:19 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$'
		|| c == '\'' || c == '"');
}

static int	handle_quotes_and_special(const char *input, size_t *index,
				int *in_quotes, char *quote_char)
{
	int	found_equals;

	found_equals = 0;
	if ((input[*index] == '"' || input[*index] == '\'') && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_char = input[*index];
	}
	else if (input[*index] == *quote_char && *in_quotes)
		*in_quotes = 0;
	if (input[*index] == '=')
		found_equals = 1;
	return (found_equals);
}

char	*extract_word(const char *input, size_t *index,
			int is_delimiter_expected)
{
	size_t	start;
	char	*word;
	int		found_equals;
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	start = *index;
	found_equals = 0;
	quote_char = 0;
	while (input[*index] && (!ft_isspace(input[*index]) || in_quotes))
	{
		if (handle_quotes_and_special(input, index, &in_quotes, &quote_char))
			found_equals = 1;
		if (!is_delimiter_expected && !found_equals && *index > start
			&& is_special_char(input[*index]) && input[*index] != '$'
			&& !in_quotes)
			break ;
		(*index)++;
	}
	if (*index == start) // Prevent empty strings being returned
		return (NULL);
	word = ft_strndup(input + start, *index - start);
	return (word);
}

t_token	*extract_operator(const char *input, size_t *index)
{
	if (input[*index] == '|')
	{
		(*index)++;
		return (create_token(TOKEN_PIPE, "|"));
	}
	if (input[*index] == '>' || input[*index] == '<')
	{
		if (input[*index + 1] == '>')
			return ((*index) += 2, create_token(TOKEN_APPEND_OUT, ">>"));
		else if (input[*index] == '>')
			return ((*index)++, create_token(TOKEN_REDIR_OUT, ">"));
		else if (input[*index + 1] == '<')
			return ((*index) += 2, create_token(TOKEN_HEREDOC, "<<"));
		else
			return ((*index)++, create_token(TOKEN_REDIR_IN, "<"));
	}
	return (NULL);
}

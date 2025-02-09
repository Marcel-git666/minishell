/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:46:37 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/09 14:03:02 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$'
		|| c == '\'' || c == '"');
}

char	*extract_word(const char *input, size_t *index)
{
	size_t	start;
	char	*word;

	start = *index;
	while (input[*index] && !ft_isspace(input[*index])
		&& (!is_special_char(input[*index]) || input[*index] == '-'
			|| (*index > start && input[*index - 1] == '-')
			|| (*index == start && (input[*index] == '/'
					|| input[*index] == '~'))))
	{
		(*index)++;
	}
	word = ft_strndup(input + start, *index - start);
	return (word);
}

char	*extract_quoted_string(const char *input, size_t *index)
{
	char	quote;
	char	*str;
	size_t	start;

	quote = input[(*index)++];
	start = *index;
	while (input[*index] && input[*index] != quote)
		(*index)++;
	str = ft_strndup(input + start, *index - start);
	if (input[*index] == quote)
		(*index)++;
	return (str);
}

t_token	*extract_operator(const char *input, size_t *index)
{
	if (input[*index] == '|')
	{
		(*index)++;
		return (create_token(TOKEN_PIPE, "|"));
	}
	else if (input[*index] == '>')
	{
		if (input[*index + 1] == '>')
		{
			(*index) += 2;
			return (create_token(TOKEN_APPEND_OUT, ">>"));
		}
		else
		{
			(*index)++;
			return (create_token(TOKEN_REDIR_OUT, ">"));
		}
	}
	else if (input[*index] == '<')
	{
		if (input[*index + 1] == '<')
		{
			(*index) += 2;
			return (create_token(TOKEN_HEREDOC, "<<"));
		}
		else
		{
			(*index)++;
			return (create_token(TOKEN_REDIR_IN, "<"));
		}
	}
	return (NULL);
}

char	*extract_env_var(const char *input, size_t *index)
{
	size_t	start;

	start = ++(*index);
	while (ft_isalnum(input[*index]) || input[*index] == '_')
		(*index)++;
	return (ft_strndup(input + start, *index - start));
}

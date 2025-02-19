/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:46:37 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/19 17:37:08 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$'
		|| c == '\'' || c == '"');
}

char	*extract_word(const char *input, size_t *index, int is_delimiter_expected)
{
	size_t	start;
	char	*word;
	int		found_equals;

	start = *index;
	found_equals = 0;
	while (input[*index] && !ft_isspace(input[*index]))
	{
		if (input[*index] == '=' && !found_equals)
			found_equals = 1;
		if (!is_delimiter_expected && !found_equals && *index > start
				&& is_special_char(input[*index]) && input[*index] != '$')
			break ;
		(*index)++;
	}
	if (*index == start)  // Prevent empty strings being returned
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

char	*extract_env_var(const char *input, size_t *index)
{
	size_t	start;

	start = ++(*index);
	if (input[*index] == '?')
	{
		(*index)++;	// Skip the ?
		return (ft_strdup("?"));
	}
	while (ft_isalnum(input[*index]) || input[*index] == '_')
		(*index)++;
	return (ft_strndup(input + start, *index - start));
}

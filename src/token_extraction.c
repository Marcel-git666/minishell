/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:46:37 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/11 21:48:50 by marcel           ###   ########.fr       */
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
	// Handle ${VAR}
    if (input[*index] == '{')
    {
        (*index)++; // Skip {
        start = *index;
        while (input[*index] && input[*index] != '}')
            (*index)++;
        if (input[*index] != '}')
            return (error_message("syntax error: missing closing brace"), NULL);
        char *var_name = ft_strndup(input + start, *index - start);
        (*index)++; // Skip }
		return (var_name);
    }
	if (input[*index] == '?')
	{
		(*index)++;	// Skip the ?
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(input[*index]) && input[*index] != '_')
		return (error_message("syntax error: invalid environment variable name"), NULL);
	(*index)++;
	while (ft_isalnum(input[*index]) || input[*index] == '_')
		(*index)++;
	return (ft_strndup(input + start, *index - start));
}

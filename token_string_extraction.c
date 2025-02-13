/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_string_extraction.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:40:18 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/13 19:09:45 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_single_quoted_string(t_lexer *lexer)
{
	char	quote;
	char	*str;
	size_t	start;

	quote = lexer->input[(lexer->i)++];
	start = lexer->i;
	while (lexer->input[lexer->i] && lexer->input[lexer->i] != quote)
		(lexer->i)++;
	if (lexer->input[lexer->i] == '\0')  // ❌ No closing quote found
	{
		error_message("syntax error: missing closing quote");
		return (NULL);
	}
	str = ft_strndup(lexer->input + start, lexer->i - start);
	(lexer->i)++; // Skip closing quote
	return (str);
}

char	*extract_double_quoted_string(t_lexer *lexer)
{
	char	quote;
	size_t	start;
	char	*env_name;

	quote = lexer->input[(lexer->i)++];
	start = lexer->i;
	while (lexer->input[lexer->i] && lexer->input[lexer->i] != quote)
	{
		if (lexer->input[lexer->i] == '$') // Found env variable inside quotes
		{
			if (lexer->i > start) // Extract the part before `$`
				add_token(&(lexer->tokens), create_token(TOKEN_STRING,
					ft_substr(lexer->input, start, lexer->i - start)));
			env_name = extract_env_var(lexer->input, &(lexer->i));
			add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env_name));
			free(env_name);
			start = lexer->i;
			continue ;
		}
		(lexer->i)++;
	}
	if (lexer->input[lexer->i] == '\0') // ❌ No closing quote
	{
		error_message("syntax error: missing closing quote");
		return (NULL);
	}
	if (lexer->i > start)
		add_token(&(lexer->tokens), create_token(TOKEN_STRING,
			ft_substr(lexer->input, start, lexer->i - start)));
	(lexer->i)++; // Skip closing quote
	return (ft_strdup(""));
}

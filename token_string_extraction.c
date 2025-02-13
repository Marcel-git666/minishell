/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_string_extraction.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:40:18 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/13 19:34:30 by mmravec          ###   ########.fr       */
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
	if (lexer->input[lexer->i] == '\0')
		return (error_message("syntax error: missing closing quote"), NULL);
	str = ft_strndup(lexer->input + start, lexer->i - start);
	(lexer->i)++; // Skip closing quote
	return (str);
}

static void	extract_text_before_env_var(t_lexer *lexer, size_t *start)
{
	if (lexer->i > *start)
		add_token(&(lexer->tokens), create_token(TOKEN_STRING,
				ft_substr(lexer->input, *start, lexer->i - *start)));
	*start = lexer->i;
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
			extract_text_before_env_var(lexer, &start);
			env_name = extract_env_var(lexer->input, &(lexer->i));
			add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env_name));
			free(env_name);
			start = lexer->i;
			continue ;
		}
		(lexer->i)++;
	}
	if (lexer->input[lexer->i] == '\0')
		return (error_message("syntax error: missing closing quote"), NULL);
	extract_text_before_env_var(lexer, &start);
	(lexer->i)++;
	return (ft_strdup(""));
}

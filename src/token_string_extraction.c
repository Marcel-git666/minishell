/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_string_extraction.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:40:18 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/10 21:25:04 by marcel           ###   ########.fr       */
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
	char	*substr;
	t_token	*token;

	if (lexer->i > *start)
	{
		substr = ft_substr(lexer->input, *start, lexer->i - *start);
		token = create_token(TOKEN_DOUBLE_QUOTED, substr);
		add_token(&(lexer->tokens), token);
		free(substr);
	}
	*start = lexer->i;
}

static void process_env_var(t_lexer *lexer, size_t *start)
{
	char *env_name;

	extract_text_before_env_var(lexer, start);
	env_name = extract_env_var(lexer->input, &(lexer->i));
	add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env_name));
	free(env_name);
	*start = lexer->i;
}

char	*extract_double_quoted_string(t_lexer *lexer)
{
	char	quote;
	size_t	start;
	char	*empty_str;

	quote = lexer->input[(lexer->i)++];
	start = lexer->i;
	while (lexer->input[lexer->i] && lexer->input[lexer->i] != quote)
	{
		if (lexer->input[lexer->i] == '$') // Found env variable inside quotes
		{
			process_env_var(lexer, &start);
			continue ;
		}
		(lexer->i)++;
	}
	if (lexer->input[lexer->i] == '\0')
		return (error_message("syntax error: missing closing quote"), NULL);
	extract_text_before_env_var(lexer, &start);
	(lexer->i)++;
	empty_str = ft_strdup("");
	return (empty_str);
}

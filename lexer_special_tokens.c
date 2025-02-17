/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_special_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:59:03 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/17 10:39:02 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static int	handle_pipe_token(t_lexer *lexer, int *is_first_word)
{
	if (lexer->i == 0 || lexer->input[lexer->i + 1] == '\0')
		return (error_message("syntax error: `|` cannot"
				" start or end a command"), -1);
	if (!ft_isalpha(lexer->input[lexer->i + 1])
		&& lexer->input[lexer->i + 1] == '\0')
		return (error_message("syntax error: `|` must be followed "
				"by a valid command"), -1);
	add_token(&(lexer->tokens), create_token(TOKEN_PIPE, "|"));
	lexer->i++;
	*is_first_word = 1;
	return (0);
}

static int	handle_quote_token(t_lexer *lexer)
{
	char	*quoted;

	if (lexer->input[lexer->i] == '\'')
		quoted = extract_single_quoted_string(lexer);
	else
		quoted = extract_double_quoted_string(lexer);
	if (!quoted)
		return (-1);
	if (ft_strlen(quoted) > 0)
	{
		add_token(&(lexer->tokens), create_token(TOKEN_STRING, quoted));
		free(quoted);
	}
	return (1);
}

int	handle_special_tokens(t_lexer *lexer, int *is_first_word)
{
	char	*env;

	if (lexer->input[lexer->i] == '\'' || lexer->input[lexer->i] == '\"')
		handle_quote_token(lexer);
	else if (lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
		return (process_redirections(lexer));
	else if (lexer->input[lexer->i] == '|')
		return (handle_pipe_token(lexer, is_first_word));
	else if (lexer->input[lexer->i] == '$')
	{
		env = extract_env_var(lexer->input, &(lexer->i));
		add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env));
		free(env);
	}
	else
		(lexer->i)++;
	return (0);
}


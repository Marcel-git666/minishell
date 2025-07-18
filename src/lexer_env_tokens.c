/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_env_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:53:51 by marcel            #+#    #+#             */
/*   Updated: 2025/07/19 00:04:04 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Checks if current position should create a compound token
 * Returns 1 if next character continues the token
 */
static int	should_create_compound(t_lexer *lexer)
{
	if (!lexer->input[lexer->i] || ft_isspace(lexer->input[lexer->i]))
		return (0);
	if (lexer->input[lexer->i] == '$')
		return (1);
	if (!is_special_char(lexer->input[lexer->i]))
		return (1);
	return (0);
}

/*
 * Creates compound token containing environment variables and text
 */
static int	create_compound_env_token(t_lexer *lexer, size_t start_pos)
{
	char	*compound;

	compound = extract_compound_token(lexer, start_pos);
	add_token(&(lexer->tokens), create_token(TOKEN_ARG, compound));
	free(compound);
	return (0);
}

/*
 * Creates regular environment variable token
 */
static int	create_env_token(t_lexer *lexer, char *env)
{
	if (ft_strncmp(env, "?", 2) == 0)
		add_token(&(lexer->tokens), create_token(TOKEN_EXIT_CODE, "?"));
	else
		add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env));
	return (0);
}

/*
 * Handles environment variable tokens ($VAR, $?, etc.)
 * Supports both simple and compound tokens
 */
static int	handle_env_token(t_lexer *lexer, int *is_first_word)
{
	char	*env;
	size_t	start_pos;

	start_pos = lexer->i;
	env = extract_env_var(lexer->input, &(lexer->i));
	if (!env)
		return (-1);
	if (should_create_compound(lexer))
		create_compound_env_token(lexer, start_pos);
	else
		create_env_token(lexer, env);
	if (*is_first_word)
		*is_first_word = 0;
	free(env);
	return (0);
}

/*
 * Main dispatcher for special token handling
 * Routes to appropriate handler based on character type
 */
int	handle_special_tokens(t_lexer *lexer, int *is_first_word)
{
	if (lexer->input[lexer->i] == '\'' || lexer->input[lexer->i] == '\"')
		return (handle_quote_token(lexer, is_first_word));
	else if (lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
		return (process_redirections(lexer));
	else if (lexer->input[lexer->i] == '|')
		return (handle_pipe_token(lexer, is_first_word));
	else if (lexer->input[lexer->i] == '$')
		return (handle_env_token(lexer, is_first_word));
	else
		(lexer->i)++;
	return (0);
}

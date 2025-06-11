/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_special_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:59:03 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/10 21:31:58 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static int	handle_pipe_token(t_lexer *lexer, int *is_first_word)
{
	int	j;
	int	k;

	if (*is_first_word)
		return (error_message("syntax error: `|` cannot start a command"), -1);
	j = lexer->i - 1;
	while (j >= 0 && ft_isspace(lexer->input[j]))
		j--;
	k = lexer->i + 1;
	while (lexer->input[k] && ft_isspace(lexer->input[k]))
		k++;
	if (lexer->input[k] == '|')
		return (error_message("syntax error: consecutive pipes are not allowed"),
			-1);
	lexer->i++;
	while (lexer->input[lexer->i] && ft_isspace(lexer->input[lexer->i]))
		lexer->i++;
	if (j < 0 || !lexer->input[lexer->i])
		return (error_message("syntax error: `|` cannot start or end a command"),
			-1);
	add_token(&(lexer->tokens), create_token(TOKEN_PIPE, "|"));
	*is_first_word = 1;
	return (0);
}

static int handle_quote_token(t_lexer *lexer)
{
    char *quoted;
    
    if (lexer->input[lexer->i] == '\'')
    {
        quoted = extract_single_quoted_string(lexer);
        if (!quoted)
            return (-1);
        if (ft_strlen(quoted) > 0)
            add_token(&(lexer->tokens), create_token(TOKEN_SINGLE_QUOTED, quoted));
    }
    else
    {
        quoted = extract_double_quoted_string(lexer);
        if (!quoted)
            return (-1);
        if (ft_strlen(quoted) > 0)
            add_token(&(lexer->tokens), create_token(TOKEN_DOUBLE_QUOTED, quoted));
    }
    
    free(quoted);
    return (0);
}

int	handle_special_tokens(t_lexer *lexer, int *is_first_word)
{
	char	*env;

	if (lexer->input[lexer->i] == '\'' || lexer->input[lexer->i] == '\"')
		return (handle_quote_token(lexer));
	else if (lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
		return (process_redirections(lexer));
	else if (lexer->input[lexer->i] == '|')
		return (handle_pipe_token(lexer, is_first_word));
	else if (lexer->input[lexer->i] == '$')
	{
		env = extract_env_var(lexer->input, &(lexer->i));
		if (!env)
			return (-1);
		if (ft_strncmp(env, "?", 2) == 0)
			add_token(&(lexer->tokens), create_token(TOKEN_EXIT_CODE, "?"));
		else
			add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env));
		free(env);
	}
	else
		(lexer->i)++;
	return (0);
}


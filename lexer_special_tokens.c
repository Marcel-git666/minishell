/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_special_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:59:03 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/17 10:12:44 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static void	add_redirection_token(t_lexer *lexer, t_token_type type,
		char *symbol)
{
	lexer->i += ft_strlen(symbol);
	add_token(&(lexer->tokens), create_token(type, symbol));
	lexer->is_file_expected = 1;
}

static int	add_heredoc_token(t_lexer *lexer, int *is_first_word)
{
	lexer->i += 2;
	add_token(&(lexer->tokens), create_token(TOKEN_HEREDOC, "<<"));
	skip_whitespace(lexer->input, &(lexer->i));
	if (!ft_isalnum(lexer->input[lexer->i]))
	{
		error_message("syntax error: expected delimiter after <<");
		return (-1);
	}
	lexer->is_delimiter_expected = 1;
	*is_first_word = 1;
	return (1);
}

int	process_redirections(t_lexer *lexer, int *is_first_word)
{
	if ((lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
		&& lexer->input[lexer->i + 1] == lexer->input[lexer->i]
		&& lexer->input[lexer->i + 2] != '\0'
		&& !ft_isspace(lexer->input[lexer->i + 2])
		&& !ft_isalnum(lexer->input[lexer->i + 2]))
		return (error_message("syntax error near unexpected token"), -1);
	if (lexer->input[lexer->i] == '<' && lexer->input[lexer->i + 1] == '<')
	{
		if (add_heredoc_token(lexer, is_first_word) == -1)
			return (-1);
	}
	else if (lexer->input[lexer->i] == '>' && lexer->input[lexer->i + 1] == '>')
		add_redirection_token(lexer, TOKEN_APPEND_OUT, ">>");
	else if (lexer->input[lexer->i] == '>')
		add_redirection_token(lexer, TOKEN_REDIR_OUT, ">");
	else if (lexer->input[lexer->i] == '<')
		add_redirection_token(lexer, TOKEN_REDIR_IN, "<");
	else
		return (-1);
	if (!lexer->is_delimiter_expected)
		*is_first_word = 0;
	return (0);
}

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

int	handle_special_tokens(t_lexer *lexer, int *is_first_word)
{
	char	*env;
	char	*quoted;

	if (lexer->input[lexer->i] == '\'' || lexer->input[lexer->i] == '\"')
	{
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
	}
	else if (lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
	{
		if (process_redirections(lexer, is_first_word) == -1)
			return (-1);
	}
	else if (lexer->input[lexer->i] == '|' && lexer->input[lexer->i + 1] == '|')
	{
		error_message("syntax error near unexpected token `||`");
		return (-1);
	}
	else if (lexer->input[lexer->i] == '|')
	{
		if (handle_pipe_token(lexer, is_first_word) == -1)
			return (-1);
	}
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


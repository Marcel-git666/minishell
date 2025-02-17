/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_process_redirections.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:21:07 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/17 11:39:39 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static void	add_redirection_token(t_lexer *lexer, char *token_str,
		int token_type, int expect_file)
{
	add_token(&(lexer->tokens), create_token(token_type, token_str));
	lexer->is_file_expected = expect_file;
	lexer->i += ft_strlen(token_str);
}

static int	handle_append_out(t_lexer *lexer)
{
	if (!ft_isspace(lexer->input[lexer->i + 2])
		&& lexer->input[lexer->i + 2] != '\0')
		return (error_message("syntax error: invalid redirection operator"),
			-1);
	add_redirection_token(lexer, ">>", TOKEN_APPEND_OUT, 1);
	return (0);
}

static int	handle_heredoc(t_lexer *lexer)
{
	if (!ft_isspace(lexer->input[lexer->i + 2])
		&& lexer->input[lexer->i + 2] != '\0')
		return (error_message("syntax error: invalid redirection operator"),
			-1);
	add_redirection_token(lexer, "<<", TOKEN_HEREDOC, 0);
	skip_whitespace(lexer->input, &(lexer->i));
	if (lexer->input[lexer->i] == '\0')
		return (error_message("syntax error: expected delimiter after <<"), -1);
	lexer->is_delimiter_expected = 1;
	return (0);
}

int	process_redirections(t_lexer *lexer)
{
	if (lexer->input[lexer->i] == '>' && lexer->input[lexer->i + 1] == '>')
		return (handle_append_out(lexer));
	else if (lexer->input[lexer->i] == '>')
		return (add_redirection_token(lexer, ">", TOKEN_REDIR_OUT, 1), 0);
	else if (lexer->input[lexer->i] == '<' && lexer->input[lexer->i + 1] == '<')
		return (handle_heredoc(lexer));
	else if (lexer->input[lexer->i] == '<')
		return (add_redirection_token(lexer, "<", TOKEN_REDIR_IN, 1), 0);
	return (-1);
}

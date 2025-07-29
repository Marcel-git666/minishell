/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_process_redirections.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:21:07 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/29 22:05:12 by mmravec          ###   ########.fr       */
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

static int	handle_single_redirection(t_lexer *lexer, char *token,
		int token_type)
{
	size_t	next_pos;

	next_pos = lexer->i + 1;
	if (check_next_token(lexer, next_pos) == -1)
		return (-1);
	return (add_redirection_token(lexer, token, token_type, 1), 0);
}

static int	handle_append_out(t_lexer *lexer)
{
	size_t	next_pos;

	if (lexer->input[lexer->i + 2] == '>')
		return (error_message("syntax error: invalid redirection operator"),
			-1);
	next_pos = lexer->i + 2;
	if (check_next_token(lexer, next_pos) == -1)
		return (-1);
	add_redirection_token(lexer, ">>", TOKEN_APPEND_OUT, 1);
	return (0);
}

static int	handle_heredoc(t_lexer *lexer)
{
	if (lexer->input[lexer->i + 2] == '<')
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
		return (handle_single_redirection(lexer, ">", TOKEN_REDIR_OUT));
	else if (lexer->input[lexer->i] == '<' && lexer->input[lexer->i + 1] == '<')
		return (handle_heredoc(lexer));
	else if (lexer->input[lexer->i] == '<')
		return (handle_single_redirection(lexer, "<", TOKEN_REDIR_IN));
	return (-1);
}

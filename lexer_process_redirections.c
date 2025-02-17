/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_process_redirections.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:21:07 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/17 11:16:15 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

void	add_redirection_token(t_lexer *lexer, char *token_str,
		int token_type, int expect_file)
{
	add_token(&(lexer->tokens), create_token(token_type, token_str));
	lexer->is_file_expected = expect_file;
	lexer->i += ft_strlen(token_str);
}

int	process_redirections(t_lexer *lexer)
{
	if (lexer->input[lexer->i] == '>' && lexer->input[lexer->i + 1] == '>')
		add_redirection_token(lexer, ">>", TOKEN_APPEND_OUT, 1);
	else if (lexer->input[lexer->i] == '>')
		add_redirection_token(lexer, ">", TOKEN_REDIR_OUT, 1);
	else if (lexer->input[lexer->i] == '<' && lexer->input[lexer->i + 1] == '<')
	{
		add_redirection_token(lexer, "<<", TOKEN_HEREDOC, 0);
		skip_whitespace(lexer->input, &(lexer->i));
		if (lexer->input[lexer->i] == '\0')
		{
			error_message("syntax error: expected delimiter after <<");
			return (-1);
		}
		lexer->is_delimiter_expected = 1;
	}
	else if (lexer->input[lexer->i] == '<')
		add_redirection_token(lexer, "<", TOKEN_REDIR_IN, 1);
	else
		return (-1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:17:10 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/13 17:59:16 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Maps token type to redirection type enum
 */
static t_redir_type	get_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_APPEND_OUT)
		return (REDIR_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}


/*
 * Parses redirection tokens into AST redirection node
 * Creates both redirection structure and AST node, handles recursive 
 * redirections
 */
t_redirection	*parse_redirection(t_parser *parser)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = get_redir_type(parser->current_token->type);
	redir->next = NULL;
	get_next_token(parser); // Posuneme se na název souboru/delimiter
	if (!parser->current_token)
	{
		set_parser_error(parser, "syntax error near unexpected token `newline'");
		return (free(redir), NULL);
	}
	redir->file_or_delimiter = ft_strdup(parser->current_token->value);
	if (!redir->file_or_delimiter)
		return (free(redir), NULL);
	get_next_token(parser); // Posuneme se za název souboru
	return (redir);
}

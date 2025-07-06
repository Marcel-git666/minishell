/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:17:10 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/05 16:35:53 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_ast_node	*parse_redirection(t_parser *parser)
{
	t_ast_node		*node;
	t_redirection	*redir;
	t_token_type	redir_type;

	// Save the redirection type
	redir_type = parser->current_token->type;
	// Create the redirection structure
	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);

	// Set redirection type
	if (redir_type == TOKEN_REDIR_IN)
		redir->type = REDIR_IN;
	else if (redir_type == TOKEN_REDIR_OUT)
		redir->type = REDIR_OUT;
	else if (redir_type == TOKEN_APPEND_OUT)
		redir->type = REDIR_APPEND;
	else if (redir_type == TOKEN_HEREDOC)
		redir->type = REDIR_HEREDOC;
	// Move to the next token (should be file or delimiter)
	get_next_token(parser);
		
	// Check if we have a filename token
	if (!parser->current_token || (parser->current_token->type != TOKEN_FILE
			&& parser->current_token->type != TOKEN_DELIMITER))
	{
		set_parser_error(parser, "syntax error: expected filename or delimiter after redirection");
		free(redir);
		return (NULL);
	}

	// Save the filename
	redir->file_or_delimiter = ft_strdup(parser->current_token->value);
	redir->next = NULL;
	
	// Move past the filename
	get_next_token(parser);
	
	// Create the AST node
	node = malloc(sizeof(t_ast_node));
	if (!node)
	{
		free(redir->file_or_delimiter);
		free(redir);
		return (NULL);
	}
	node->type = NODE_REDIR;
	node->u_content.redir.redir = redir;
	node->u_content.redir.child = NULL;  // Will be set later
	node->next = NULL;

	// If the next token is a command, parse it
	if (parser->current_token && parser->current_token->type == TOKEN_CMD)
	{
		node->u_content.redir.child = parse_command(parser);
	}

	// If the next token is another redirection, handle it recursively
	if (parser->current_token && is_redirection_token(parser->current_token->type))
	{
		t_ast_node *next_redir = parse_redirection(parser);

		// Link the redirections
		if (next_redir && next_redir->type == NODE_REDIR)
		{
			// If we already have a command, use it
			if (node->u_content.redir.child)
				next_redir->u_content.redir.child = node->u_content.redir.child;
        	node->u_content.redir.child = next_redir;
		}
	}
	return (node);
}

t_ast_node *attach_redirection_to_command(t_ast_node *cmd_node, t_parser *parser)
{
	t_ast_node	*redir_node = parse_redirection(parser);
	t_ast_node	*original;

	if (!redir_node)
		return (cmd_node);  // Return original if redirection parsing failed

	original = redir_node;
	while (redir_node->u_content.redir.child)
		redir_node = redir_node->u_content.redir.child;
	// Attach the command to the redirection
	redir_node->u_content.redir.child = cmd_node;
	return (original);
}

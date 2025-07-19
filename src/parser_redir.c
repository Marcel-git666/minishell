/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:17:10 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 00:31:13 by marcel           ###   ########.fr       */
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
 * Creates and initializes redirection structure
 */
static t_redirection	*create_redirection(t_parser *parser)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = get_redir_type(parser->current_token->type);
	redir->next = NULL;
	get_next_token(parser);
	if (!parser->current_token || (parser->current_token->type != TOKEN_FILE
			&& parser->current_token->type != TOKEN_DELIMITER))
	{
		set_parser_error(parser,
			"syntax error: expected filename or delimiter after redirection");
		free(redir);
		return (NULL);
	}
	redir->file_or_delimiter = ft_strdup(parser->current_token->value);
	get_next_token(parser);
	return (redir);
}

/*
 * Handles recursive redirection parsing and linking
 */
static void	handle_recursive_redirections(t_ast_node *node, t_parser *parser)
{
	t_ast_node	*next_redir;

	if (parser->current_token && parser->current_token->type == TOKEN_CMD)
		node->u_content.s_redir.child = parse_command(parser);
	if (parser->current_token
		&& is_redirection_token(parser->current_token->type))
	{
		next_redir = parse_redirection(parser);
		if (next_redir && next_redir->type == NODE_REDIR)
		{
			if (node->u_content.s_redir.child)
				next_redir->u_content.s_redir.child = node->u_content.s_redir.child;
			node->u_content.s_redir.child = next_redir;
		}
	}
}

/*
 * Parses redirection tokens into AST redirection node
 * Creates both redirection structure and AST node, handles recursive 
 * redirections
 */
t_ast_node	*parse_redirection(t_parser *parser)
{
	t_redirection	*redir;
	t_ast_node		*node;

	redir = create_redirection(parser);
	if (!redir)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
	{
		free(redir->file_or_delimiter);
		free(redir);
		return (NULL);
	}
	node->type = NODE_REDIR;
	node->u_content.s_redir.redir = redir;
	node->u_content.s_redir.child = NULL;
	node->next = NULL;
	handle_recursive_redirections(node, parser);
	return (node);
}

/*
 * Attaches redirection to existing command node
 * Returns redirection node with command as child
 */
t_ast_node	*attach_redirection_to_command(t_ast_node *cmd_node,
		t_parser *parser)
{
	t_ast_node	*redir_node;
	t_ast_node	*original;

	redir_node = parse_redirection(parser);
	if (!redir_node)
		return (cmd_node);
	original = redir_node;
	while (redir_node->u_content.s_redir.child)
		redir_node = redir_node->u_content.s_redir.child;
	redir_node->u_content.s_redir.child = cmd_node;
	return (original);
}

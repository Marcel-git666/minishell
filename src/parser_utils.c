/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:45:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 00:31:24 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Advances parser to next token in linked list
 * Returns current token before advancing
 */
t_token	*get_next_token(t_parser *parser)
{
	t_token	*current;

	current = parser->current_token;
	if (parser->current_token)
		parser->current_token = parser->current_token->next;
	return (current);
}

/*
 * Frees command node and all its components including arguments
 */
static void	free_command_node(t_ast_node *node)
{
	int	i;

	if (node->u_content.cmd.cmd)
		free(node->u_content.cmd.cmd);
	if (node->u_content.cmd.args)
	{
		i = 0;
		while (i < node->u_content.cmd.arg_count)
		{
			free(node->u_content.cmd.args[i]);
			i++;
		}
		free(node->u_content.cmd.args);
	}
	if (node->u_content.cmd.arg_token_types)
		free(node->u_content.cmd.arg_token_types);
}

/*
 * Frees redirection node and all its components
 */
static void	free_redirection_node(t_ast_node *node)
{
	if (node->u_content.s_redir.redir)
	{
		free(node->u_content.s_redir.redir->file_or_delimiter);
		free(node->u_content.s_redir.redir);
	}
	if (node->u_content.s_redir.child)
		free_ast(node->u_content.s_redir.child);
}

/*
 * Frees pipe node and recursively frees left and right branches
 */
static void	free_pipe_node(t_ast_node *node)
{
	if (node->u_content.s_pipe.left)
		free_ast(node->u_content.s_pipe.left);
	if (node->u_content.s_pipe.right)
		free_ast(node->u_content.s_pipe.right);
}

/*
 * Recursively frees AST node and all its children based on node type
 */
void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
		free_command_node(node);
	else if (node->type == NODE_PIPE)
		free_pipe_node(node);
	else if (node->type == NODE_REDIR)
		free_redirection_node(node);
	else if (node->type == NODE_ASSIGNMENT)
	{
		free(node->u_content.s_assign.name);
		free(node->u_content.s_assign.value);
	}
	free(node);
}

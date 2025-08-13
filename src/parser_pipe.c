/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:42:32 by marcel            #+#    #+#             */
/*   Updated: 2025/08/13 17:56:07 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Creates pipe AST node connecting left command with right expression
 * Left node is the command before pipe, right node is parsed from remaining 
 * tokens
 * Returns pipe node or NULL on error (invalid command after pipe)
 */
t_ast_node	*create_pipe_node(t_ast_node *left_node, t_ast_node *right_node)
{
	t_ast_node	*pipe_node;

	if (!left_node || !right_node)
	{
		free_ast(left_node);
		free_ast(right_node);
		return (NULL);
	}
	pipe_node = malloc(sizeof(t_ast_node));
	if (!pipe_node)
	{
		free_ast(left_node);
		free_ast(right_node);
		return (NULL);
	}
	pipe_node->type = NODE_PIPE;
	pipe_node->u_content.s_pipe.left = left_node;
	pipe_node->u_content.s_pipe.right = right_node;
	pipe_node->next = NULL;
	return (pipe_node);
}

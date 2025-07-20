/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:42:32 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 00:31:02 by marcel           ###   ########.fr       */
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
t_ast_node	*create_pipe_node(t_ast_node *left_node, t_parser *parser)
{
	t_ast_node	*pipe_node;

	pipe_node = malloc(sizeof(t_ast_node));
	if (!pipe_node)
		return (NULL);
	pipe_node->type = NODE_PIPE;
	pipe_node->u_content.s_pipe.left = left_node;
	pipe_node->next = NULL;
	pipe_node->u_content.s_pipe.right = parse_expression(parser);
	if (pipe_node->u_content.s_pipe.right == NULL)
	{
		set_parser_error(parser, "syntax error: invalid command after pipe");
		free(pipe_node);
		return (NULL);
	}
	return (pipe_node);
}

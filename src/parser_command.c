/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:41:37 by marcel            #+#    #+#             */
/*   Updated: 2025/08/13 18:01:28 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Parses command tokens into AST command node
 * Handles command name and all associated arguments
 */
t_ast_node	*parse_command(t_parser *parser)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->u_content.cmd.cmd = ft_strdup(parser->current_token->value);
	if (!node->u_content.cmd.cmd)
		return (free(node), NULL);
	node->u_content.cmd.cmd_token_type = parser->current_token->type;
	node->u_content.cmd.arg_count = 0;
	node->u_content.cmd.args = NULL;
	node->u_content.cmd.arg_token_types = NULL;
	node->u_content.cmd.redirections = NULL;
	node->next = NULL;
	return (node);
}

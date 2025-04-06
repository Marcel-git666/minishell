/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:45:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/04/06 16:28:17 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_token	*get_next_token(t_parser *parser)
{
	t_token *current;

	current = parser->current_token;
	if (parser->current_token)
		parser->current_token = parser->current_token->next;
	return (current);
}

void	free_ast(t_ast_node *node)
{
	int		i;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		if (node->u_content.cmd.cmd)
			free(node->u_content.cmd.cmd);
		if (node->u_content.cmd.args)
		{
			i = -1;
			while (++i < node->u_content.cmd.arg_count)
				free(node->u_content.cmd.args[i]);
			free(node->u_content.cmd.args);
		}
	}
	if (node->type == NODE_PIPE)
	{
		if (node->u_content.pipe.left)
			free_ast(node->u_content.pipe.left);
		if (node->u_content.pipe.right)
			free_ast(node->u_content.pipe.right);
	}
	free(node);
}

void	print_ast(t_ast_node *node, int depth)
{
	for (int i = 0; i < depth; i++)
		printf("  ");

	if (!node)
	{
		printf("(null)\n");
		return ;
	}
	if (node->type == NODE_COMMAND)
	{
		printf("COMMAND: %s\n", node->u_content.cmd.cmd);
		for (int i = 0; i < node->u_content.cmd.arg_count; i++)
		{
			for (int j = 0; j < depth + 1; j++)
				printf("  ");
			printf("ARG: %s\n", node->u_content.cmd.args[i]);
		}
	}
	else if (node->type == NODE_PIPE)
	{
		printf("PIPE\n");
		for (int j = 0; j < depth + 1; j++)
			printf("  ");
		printf("LEFT:\n");
		print_ast(node->u_content.pipe.left, depth + 2);
		for (int j = 0; j < depth + 1; j++)
			printf("  ");
		printf("RIGHT:\n");
		print_ast(node->u_content.pipe.right, depth + 2);
	}
	// Handle other node types as needed
}

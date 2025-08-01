/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:41:37 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 00:01:49 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Checks if token type is a valid argument token and counts arguments
 * Returns the number of argument tokens following the command token
 */
static int	count_arguments(t_token *cmd_token)
{
	int		count;
	t_token	*current;

	count = 0;
	current = cmd_token->next;
	while (current && (current->type == TOKEN_ARG
			|| current->type == TOKEN_DOUBLE_QUOTED
			|| current->type == TOKEN_SINGLE_QUOTED
			|| current->type == TOKEN_ENV_VAR
			|| current->type == TOKEN_EXIT_CODE))
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
 * Creates and initializes basic AST node structure for command
 */
static t_ast_node	*create_command_node(t_token *cmd_token, int arg_count)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->u_content.cmd.cmd = ft_strdup(cmd_token->value);
	node->u_content.cmd.arg_count = arg_count;
	node->u_content.cmd.cmd_token_type = cmd_token->type;
	node->next = NULL;
	return (node);
}

/*
 * Allocates memory for argument arrays
 * Returns 0 on success, -1 on failure
 */
static int	allocate_argument_arrays(t_ast_node *node, int arg_count)
{
	if (arg_count > 0)
	{
		node->u_content.cmd.args = malloc((arg_count + 1) * sizeof(char *));
		node->u_content.cmd.arg_token_types = malloc(arg_count * sizeof(int));
		if (!node->u_content.cmd.args || !node->u_content.cmd.arg_token_types)
			return (-1);
	}
	else
	{
		node->u_content.cmd.args = NULL;
		node->u_content.cmd.arg_token_types = NULL;
	}
	return (0);
}

/*
 * Fills argument arrays with token values and types
 */
static void	fill_argument_arrays(t_ast_node *node, t_parser *parser)
{
	int	i;

	i = 0;
	while (i < node->u_content.cmd.arg_count)
	{
		node->u_content.cmd.args[i] = ft_strdup(parser->current_token->value);
		node->u_content.cmd.arg_token_types[i] = parser->current_token->type;
		parser->current_token = parser->current_token->next;
		i++;
	}
}

/*
 * Parses command tokens into AST command node
 * Handles command name and all associated arguments
 */
t_ast_node	*parse_command(t_parser *parser)
{
	t_ast_node	*node;
	t_token		*cmd_token;
	int			arg_count;

	cmd_token = parser->current_token;
	arg_count = count_arguments(cmd_token);
	node = create_command_node(cmd_token, arg_count);
	if (!node)
		return (NULL);
	if (allocate_argument_arrays(node, arg_count) == -1)
	{
		free(node->u_content.cmd.cmd);
		free(node);
		return (NULL);
	}
	parser->current_token = cmd_token->next;
	fill_argument_arrays(node, parser);
	return (node);
}

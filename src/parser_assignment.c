/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_assignment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:43:41 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 00:31:24 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Finds the equals sign position in assignment string
 * Returns pointer to '=' character or NULL if not found
 */
static char	*find_equals_position(char *assignment, t_parser *parser)
{
	char	*equals_pos;

	equals_pos = ft_strchr(assignment, '=');
	if (!equals_pos)
	{
		set_parser_error(parser, "syntax error: invalid assignment");
		return (NULL);
	}
	return (equals_pos);
}

/*
 * Creates and initializes AST node for assignment
 * Returns allocated node or NULL on failure
 */
static t_ast_node	*create_assignment_node(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_ASSIGNMENT;
	node->next = NULL;
	return (node);
}

/*
 * Splits assignment string into variable name and value
 * Stores them in the AST node structure
 */
static void	split_assignment_parts(t_ast_node *node, char *assignment,
		char *equals_pos)
{
	node->u_content.s_assign.name = ft_strndup(assignment,
			equals_pos - assignment);
	node->u_content.s_assign.value = ft_strdup(equals_pos + 1);
}

/*
 * Handles cleanup on assignment parsing failure
 */
static void	cleanup_assignment_error(t_ast_node *node)
{
	if (node)
	{
		if (node->u_content.s_assign.name)
			free(node->u_content.s_assign.name);
		if (node->u_content.s_assign.value)
			free(node->u_content.s_assign.value);
		free(node);
	}
}

/*
 * Parses assignment token (VAR=value) into AST assignment node
 * Returns AST node or NULL on error
 */
t_ast_node	*parse_assignment(t_parser *parser)
{
	t_ast_node	*node;
	char		*assignment;
	char		*equals_pos;

	assignment = parser->current_token->value;
	equals_pos = find_equals_position(assignment, parser);
	if (!equals_pos)
		return (NULL);
	node = create_assignment_node();
	if (!node)
		return (NULL);
	split_assignment_parts(node, assignment, equals_pos);
	if (!node->u_content.s_assign.name || !node->u_content.s_assign.value)
	{
		cleanup_assignment_error(node);
		return (NULL);
	}
	get_next_token(parser);
	return (node);
}

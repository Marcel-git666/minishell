/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:37:49 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 00:02:40 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Initializes parser structure with token list and resets error state
 */
void	init_parser(t_token *tokens, t_parser *parser)
{
	parser->current_token = tokens;
	parser->token_head = tokens;
	parser->error = 0;
	parser->error_msg = NULL;
}

/*
 * Main parsing entry point - converts token list to AST
 * Returns AST root node or NULL on error
 */
t_ast_node	*parse_tokens(t_token *tokens)
{
	t_ast_node	*ast_node;
	t_parser	parser;

	if (!tokens)
		return (NULL);
	ast_node = NULL;
	init_parser(tokens, &parser);
	ast_node = parse_expression(&parser);
	if (parser.error)
	{
		if (ast_node)
			free_ast(ast_node);
		return (NULL);
	}
	return (ast_node);
}

/*
 * Handles command parsing and potential redirection attachment
 */
static t_ast_node	*parse_command_with_redirections(t_parser *parser)
{
	t_ast_node	*node;

	node = parse_command(parser);
	if (parser->current_token
		&& is_redirection_token(parser->current_token->type))
		node = attach_redirection_to_command(node, parser);
	return (node);
}

/*
 * Determines which type of node to parse based on current token
 * Returns appropriate AST node or NULL on error
 */
static t_ast_node	*parse_node_by_type(t_parser *parser)
{
	if (is_redirection_token(parser->current_token->type))
		return (parse_redirection(parser));
	else if (parser->current_token->type == TOKEN_CMD
		|| parser->current_token->type == TOKEN_ENV_VAR
		|| parser->current_token->type == TOKEN_EXIT_CODE)
	{
		return (parse_command_with_redirections(parser));
	}
	else if (parser->current_token->type == TOKEN_ASSIGNMENT)
		return (parse_assignment(parser));
	else
	{
		set_parser_error(parser,
			"syntax error: expected command or redirection");
		return (NULL);
	}
}

/*
 * Parses tokens into expression nodes (commands, redirections, assignments)
 * Handles pipe connections between expressions
 */
t_ast_node	*parse_expression(t_parser *parser)
{
	t_ast_node	*node;

	if (!parser->current_token)
		return (NULL);
	node = parse_node_by_type(parser);
	if (!node)
		return (NULL);
	if (parser->current_token && parser->current_token->type == TOKEN_PIPE)
	{
		get_next_token(parser);
		node = create_pipe_node(node, parser);
	}
	return (node);
}

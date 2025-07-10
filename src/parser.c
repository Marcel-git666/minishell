/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:37:49 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/23 16:08:35 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	init_parser(t_token *tokens, t_parser *parser)
{
	parser->current_token = tokens;
	parser->token_head = tokens;
	parser->error = 0;
	parser->error_msg = NULL;
}

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

t_ast_node	*parse_expression(t_parser *parser)
{
	t_ast_node	*node;

	if (!parser->current_token)
		return (NULL);
		
	// DEBUG: Zobrazit typ current tokenu
	printf("DEBUG: parse_expression current token type: %d, value: '%s'\n", 
		parser->current_token->type, parser->current_token->value);
		
	if (is_redirection_token(parser->current_token->type))
		node = parse_redirection(parser);
	else if (parser->current_token->type == TOKEN_CMD
		|| parser->current_token->type == TOKEN_ENV_VAR 
		|| parser->current_token->type == TOKEN_EXIT_CODE)
	{
		printf("DEBUG: Parsing as command (type %d)\n", parser->current_token->type);
		node = parse_command(parser);
		if (parser->current_token
			&& is_redirection_token(parser->current_token->type))
			node = attach_redirection_to_command(node, parser);
	}
	else if (parser->current_token->type == TOKEN_ASSIGNMENT)
	{
		printf("DEBUG: Parsing as assignment\n");
		node = parse_assignment(parser);
	}
	else
	{
		printf("DEBUG: ERROR - unexpected token type: %d\n", parser->current_token->type);
		
		set_parser_error(parser,
			"syntax error: expected command or redirection");
		return (NULL);
	}
	// Check if next token is a pipe
	if (parser->current_token && parser->current_token->type == TOKEN_PIPE)
	{
		get_next_token(parser);
		node = create_pipe_node(node, parser);
	}
	return (node);
}

t_ast_node	*create_pipe_node(t_ast_node *left_node, t_parser *parser)
{
	t_ast_node	*pipe_node;

	pipe_node = malloc(sizeof(t_ast_node));
	if (!pipe_node)
		return (NULL);
	pipe_node->type = NODE_PIPE;
	pipe_node->u_content.pipe.left = left_node;
	pipe_node->next = NULL;
	pipe_node->u_content.pipe.right = parse_expression(parser);
	if (pipe_node->u_content.pipe.right == NULL)
	{
		set_parser_error(parser, "syntax error: invalid command after pipe");
		free(pipe_node);
		return (NULL);
	}
	return (pipe_node);
}

t_ast_node	*parse_command(t_parser *parser)
{
	int			argument_count;
	t_ast_node	*ast_node;
	t_token		*cmd_token;
	int			i;

	i = -1;
	argument_count = 0;
	cmd_token = parser->current_token;
	// DEBUG: Zkontrolujte typ command tokenu
	printf("DEBUG: parse_command - token type: %d, value: '%s'\n", cmd_token->type, cmd_token->value);
	
	parser->current_token = parser->current_token->next;
	while (parser->current_token && (parser->current_token->type == TOKEN_ARG
			|| parser->current_token->type == TOKEN_DOUBLE_QUOTED
			|| parser->current_token->type == TOKEN_SINGLE_QUOTED
			|| parser->current_token->type == TOKEN_ENV_VAR
			|| parser->current_token->type == TOKEN_EXIT_CODE))
	{
		argument_count++;
		parser->current_token = parser->current_token->next;
	}
	parser->current_token = cmd_token;
	ast_node = malloc(sizeof(t_ast_node));
	if (!ast_node)
		return (NULL);
	ast_node->type = NODE_COMMAND;
	ast_node->u_content.cmd.cmd = ft_strdup(cmd_token->value);
	ast_node->u_content.cmd.arg_count = argument_count;
	// KRITICKÉ: Nastavit cmd_token_type
	ast_node->u_content.cmd.cmd_token_type = cmd_token->type;
	printf("DEBUG: Set cmd_token_type to: %d\n", ast_node->u_content.cmd.cmd_token_type);
	
	if (argument_count > 0)
	{
		ast_node->u_content.cmd.args
			= malloc((argument_count + 1) * sizeof(char *));
		ast_node->u_content.cmd.arg_token_types
			= malloc(argument_count * sizeof(int));
		if (!ast_node->u_content.cmd.args
			|| !ast_node->u_content.cmd.arg_token_types)
		{
			free(ast_node->u_content.cmd.cmd);
			if (ast_node->u_content.cmd.args)
				free(ast_node->u_content.cmd.args);
			if (ast_node->u_content.cmd.arg_token_types)
				free(ast_node->u_content.cmd.arg_token_types);
			free(ast_node);
			return (NULL);
		}
	}
	else
	{
		ast_node->u_content.cmd.args = NULL;
		ast_node->u_content.cmd.arg_token_types = NULL;
	}
	parser->current_token = parser->current_token->next;
	while (++i < argument_count)
	{
		ast_node->u_content.cmd.args[i]
			= ft_strdup(parser->current_token->value);
		ast_node->u_content.cmd.arg_token_types[i]
			= parser->current_token->type;
		parser->current_token = parser->current_token->next;
	}
	printf("DEBUG: parse_command finished, returning AST node\n");
	return (ast_node);
}

t_ast_node	*parse_assignment(t_parser *parser)
{
	t_ast_node	*ast_node;
	char		*assignment;
	char		*equals_pos;
	t_token		*assign_token;

	assign_token = parser->current_token;
	assignment = assign_token->value;
	
	printf("DEBUG: parse_assignment called with: '%s'\n", assignment);
	
	// Najdi pozici '='
	equals_pos = ft_strchr(assignment, '=');
	if (!equals_pos)
	{
		set_parser_error(parser, "syntax error: invalid assignment");
		return (NULL);
	}
	
	ast_node = malloc(sizeof(t_ast_node));
	if (!ast_node)
		return (NULL);
		
	ast_node->type = NODE_ASSIGNMENT;
	ast_node->next = NULL;
	
	// Rozděl na název a hodnotu
	ast_node->u_content.assign.name = ft_strndup(assignment, equals_pos - assignment);
	ast_node->u_content.assign.value = ft_strdup(equals_pos + 1);
	
	printf("DEBUG: Assignment parsed - name: '%s', value: '%s'\n", 
		ast_node->u_content.assign.name, ast_node->u_content.assign.value);
	
	// Posun na další token
	get_next_token(parser);
	
	return (ast_node);
}
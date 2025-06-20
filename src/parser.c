/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:37:49 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/14 22:05:08 by marcel           ###   ########.fr       */
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
	if (is_redirection_token(parser->current_token->type))
		node = parse_redirection(parser);
	else if (parser->current_token->type == TOKEN_CMD)
	{
		node = parse_command(parser);
		if (parser->current_token
			&& is_redirection_token(parser->current_token->type))
			node = attach_redirection_to_command(node, parser);
	}
	else
	{
		set_parser_error(parser, "syntax error: expected command or redirection");
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
	if (argument_count > 0)
	{
		ast_node->u_content.cmd.args = malloc((argument_count + 1) * sizeof(char *));
		ast_node->u_content.cmd.arg_token_types = malloc(argument_count * sizeof(int));
		if (!ast_node->u_content.cmd.args || !ast_node->u_content.cmd.arg_token_types)
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
		ast_node->u_content.cmd.arg_token_types[i] = parser->current_token->type;
		parser->current_token = parser->current_token->next;
	}
	return (ast_node);
}


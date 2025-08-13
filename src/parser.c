/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:37:49 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/13 17:53:51 by marcel           ###   ########.fr       */
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

static void	add_redir_to_list(t_redirection **list, t_redirection *new_redir)
{
	t_redirection	*current;

	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

/*
 * Nová hlavní funkce pro parsování jednoduchého příkazu.
 * Iteruje přes tokeny a sbírá argumenty a přesměrování.
 */
static t_ast_node	*parse_simple_command(t_parser *parser)
{
	t_ast_node		*cmd_node;
	t_redirection	*redir;

	cmd_node = NULL;
	while (parser->current_token && parser->current_token->type != TOKEN_PIPE)
	{
		if (is_redirection_token(parser->current_token->type))
		{
			// Získáme přesměrování z `parser_redir.c`
			redir = parse_redirection(parser); // Upravíme parse_redirection, aby vracela t_redirection*
			if (!redir)
				return (free_ast(cmd_node), NULL);
			if (!cmd_node) // Pokud je přesměrování před příkazem
			{
				cmd_node = ft_calloc(1, sizeof(t_ast_node)); // Potřebujeme uzel, kam ho uložit
				cmd_node->type = NODE_COMMAND;
			}
			add_redir_to_list(&cmd_node->u_content.cmd.redirections, redir);
		}
		else // Je to příkaz nebo argument
		{
			if (!cmd_node)
				cmd_node = parse_command(parser); // Vytvoří uzel příkazu
			else
				add_argument_to_command(cmd_node, parser->current_token); // Přidá další argument
			get_next_token(parser);
		}
	}
	return (cmd_node);
}


/*
 * Parses tokens into expression nodes (commands, redirections, assignments)
 * Handles pipe connections between expressions
 */
t_ast_node	*parse_expression(t_parser *parser)
{
	t_ast_node	*node;
	t_ast_node	*right_node;

	if (!parser->current_token)
		return (NULL);
	if (parser->current_token->type == TOKEN_ASSIGNMENT &&
		(!parser->current_token->next || parser->current_token->next->type == TOKEN_PIPE))
		return (parse_assignment(parser));
	node = parse_simple_command(parser);
	if (!node)
		return (NULL);
	if (parser->current_token && parser->current_token->type == TOKEN_PIPE)
	{
		get_next_token(parser);
		if (!parser->current_token)
		{
			set_parser_error(parser, "syntax error near unexpected token `|'");
			return (free_ast(node), NULL);
		}
		right_node = parse_expression(parser);
		if (!right_node)
			return (free_ast(node), NULL);
		node = create_pipe_node(node, right_node);
	}
	return (node);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_ast_node	*ast_node;
	t_parser	parser;

	if (!tokens)
		return (NULL);
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


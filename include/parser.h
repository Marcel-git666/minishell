/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:18:15 by mmravec           #+#    #+#             */
/*   Updated: 2025/04/05 23:11:40 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "token.h"
# include "lexer.h"

/*
 * Node types for our Abstract Syntax Tree
 */
typedef enum e_node_type
{
	NODE_COMMAND,		// Simple command (cmd + args)
	NODE_PIPE,			// Pipeline
	NODE_REDIR,			// Redirection
	NODE_ASSIGNMENT		// Variable assignment
}	t_node_type;

/*
 * Redirection type
 */
typedef enum e_redir_type
{
	REDIR_IN,		// Input redirection (<)
	REDIR_OUT,		// Output redirection (>)
	REDIR_APPEND,	// Append output (>>)
	REDIR_HEREDOC	// Here-document (<<)
}	t_redir_type;

/*
 * Redirection structure
 */
typedef struct	s_redirection
{
	t_redir_type			type;
	char					*file_or_delimiter;
	struct s_redirection	*next;
}		t_redirection;

/*
 * Command structure - represents a simple command with args and redirections
 */
typedef struct s_command
{
	char			*cmd;
	char			**args;			// NULL-terminated array of args
	int				arg_count;		// Number of arguments
	t_redirection	*redirections;
}	t_command;

/*
 * AST node - the main component of our syntax tree
 */
typedef struct s_ast_node
{
	t_node_type		type;

	// Different node contents based on type
	union
	{
		t_command			cmd;		// For NODE_COMMAND
		struct
		{								// For NODE_PIPE
			struct s_ast_node *left;
			struct s_ast_node *right;
		} pipe;
		struct {						// For NODE_REDIR
			struct s_ast_node	*child;
			t_redirection		*redir;
		} redir;
		struct
		{								// For NODE_ASSIGNMENT
			char				*name;
			char				*value;
		} assign;
	} u_content;

	struct s_ast_node *next;  // For sequential commands (CMD1 ; CMD2)
}	t_ast_node;

/*
 * Parser structure - holds state during parsing
 */
typedef struct s_parser
{
	t_token		*current_token;	// Current token being parsed
	t_token		*token_head;	// Head of token list (for cleanup)
	int			error;			// Error flag
	char		*error_msg;		// Error message
}	t_parser;

void		free_ast(t_ast_node *node);
void		init_parser(t_token *tokens, t_parser *parser);
t_ast_node	*parse_tokens(t_token *tokens);
t_ast_node	*parse_command(t_parser *parser);

#endif

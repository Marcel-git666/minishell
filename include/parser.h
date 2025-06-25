/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:18:15 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/23 16:09:30 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

// Forward declaration for token structures
struct s_token;
typedef struct s_token t_token;

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
	int             *arg_token_types; // Array of token types for each arg
	int				cmd_token_type;	// Type of the command token (TOKEN_CMD, TOKEN_ENV_VAR, etc.)
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

// parser.c
void		free_ast(t_ast_node *node);
void		init_parser(t_token *tokens, t_parser *parser);
t_ast_node	*parse_tokens(t_token *tokens);
t_ast_node	*parse_command(t_parser *parser);
t_ast_node	*parse_expression(t_parser *parser);
t_ast_node	*create_pipe_node(t_ast_node *left_node, t_parser *parser);
t_ast_node	*parse_assignment(t_parser *parser);
// parser_error.c
int			set_parser_error(t_parser *parser, const char *msg);

// parser_redir.c
t_ast_node	*parse_redirection(t_parser *parser);
t_ast_node	*attach_redirection_to_command(t_ast_node *cmd_node,
				t_parser *parser);

// parser_utils.c
t_token		*get_next_token(t_parser *parser);
void		print_ast(t_ast_node *node, int depth); // temporary

#endif

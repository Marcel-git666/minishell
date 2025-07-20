/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:18:15 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 00:37:35 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

struct					s_token;
typedef struct s_token	t_token;

typedef enum e_node_type
{
	NODE_COMMAND = 0,
	NODE_PIPE = 1,
	NODE_REDIR = 2,
	NODE_ASSIGNMENT = 3
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redirection
{
	t_redir_type			type;
	char					*file_or_delimiter;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char			*cmd;
	char			**args;
	int				arg_count;
	int				*arg_token_types;
	int				cmd_token_type;
	t_redirection	*redirections;
}	t_command;

typedef struct s_ast_node
{
	t_node_type			type;
	union
	{
		t_command				cmd;
		struct
		{
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		}	s_pipe;
		struct
		{
			struct s_ast_node	*child;
			t_redirection		*redir;
		}	s_redir;
		struct
		{
			char				*name;
			char				*value;
		}	s_assign;
	}					u_content;
	struct s_ast_node	*next;
}	t_ast_node;

typedef struct s_parser
{
	t_token	*current_token;
	t_token	*token_head;
	int		error;
	char	*error_msg;
}	t_parser;

void		init_parser(t_token *tokens, t_parser *parser);
t_ast_node	*parse_tokens(t_token *tokens);
t_ast_node	*parse_expression(t_parser *parser);
t_ast_node	*parse_command(t_parser *parser);
t_ast_node	*parse_assignment(t_parser *parser);
t_ast_node	*create_pipe_node(t_ast_node *left_node, t_parser *parser);
int			set_parser_error(t_parser *parser, const char *msg);
t_ast_node	*parse_redirection(t_parser *parser);
t_ast_node	*attach_redirection_to_command(t_ast_node *cmd_node,
				t_parser *parser);
t_token		*get_next_token(t_parser *parser);
void		free_ast(t_ast_node *node);

#endif

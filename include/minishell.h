/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2025/04/27 22:42:49 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include "string.h"
# include <ctype.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "token.h"
# include "libft.h"
# include <fcntl.h>
# include "lexer.h"
# include "parser.h"
# define HISTORY_FILE ".history"

// global variable
extern int	g_signal_received;

// builtins.c
void		builtin_exit(void);

// execution.c
void		execute_command(t_ast_node *ast_node);

// file_manipulation.c
int			open_file(char *name, char **file_content, int oflag);

// ft_strtok.c
char		*ft_strtok(char *s1, const char *s2);

// history.c
void		load_history(void);
void		save_history(void);

// lexer.c
void		skip_whitespace(const char *input, size_t *i);
t_token		*lexer(const char *input);

// lexer_process_redirections.c
int			process_redirections(t_lexer *lexer);

// lexer_special_tokens.c
int			handle_special_tokens(t_lexer *lexer, int *is_first_word);

// main.c
void		print_tokens(t_token *tokens);

// parser.c
void		free_ast(t_ast_node *node);
void		init_parser(t_token *tokens, t_parser *parser);
t_ast_node	*parse_tokens(t_token *tokens);
t_ast_node	*parse_command(t_parser *parser);
t_ast_node	*parse_expression(t_parser *parser);
t_ast_node	*create_pipe_node(t_ast_node *left_node, t_parser *parser);

// parser_error.c
int			set_parser_error(t_parser *parser, const char *msg);

// parser_redir.c
t_ast_node	*parse_redirection(t_parser *parser);
t_ast_node	*attach_redirection_to_command(t_ast_node *cmd_node,
				t_parser *parser);

// parser_utils.c
t_token		*get_next_token(t_parser *parser);
void		print_ast(t_ast_node *node, int depth); // temporary

// signals.c
void		setup_signals(void);

// token.c
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new_token);
void		free_tokens(t_token *head);
int			is_redirection_token(t_token_type type);

// token_extraction.c
int			is_special_char(char c);
char		*extract_word(const char *input, size_t *index,
				int is_delimiter_expected);
t_token		*extract_operator(const char *input, size_t *index);
char		*extract_env_var(const char *input, size_t *index);

// token_string_extraction.c
char		*extract_single_quoted_string(t_lexer *lexer);
char		*extract_double_quoted_string(t_lexer *lexer);

// shell_loop.c
void		initialize_shell(void);
void		handle_input(char *input);
void		run_shell_loop(void);

// utils.c
int			ft_isspace(int c);
char		*ft_strndup(const char *s, size_t n);
void		error_message(const char *error);
int			check_next_token(t_lexer *lexer, size_t next_pos);

#endif

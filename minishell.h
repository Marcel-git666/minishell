/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/10 22:32:38 by mmravec          ###   ########.fr       */
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
# include "libft/libft.h"
# include <fcntl.h>
# include "lexer.h"
# define HISTORY_FILE ".history"

// builtins.c
void	builtin_exit(void);

// execution.c
void	execute_command(t_token *tokens);

// file_manipulation.c
int		open_file(char *name, char **file_content, int oflag);

// ft_strtok.c
char	*ft_strtok(char *s1, const char *s2);

// history.c
void	load_history(void);
void	save_history(void);

// lexer.c
t_token	*lexer(const char *input);

// main.c
void	print_tokens(t_token *tokens);

// token_extraction.c
int		is_special_char(char c);
char	*extract_word(const char *input, size_t *index);
char	*extract_quoted_string(const char *input, size_t *index);
t_token	*extract_operator(const char *input, size_t *index);
char	*extract_env_var(const char *input, size_t *index);

// token.c
t_token	*create_token(t_token_type type, char *value);
void	add_token(t_token **head, t_token *new_token);
void	free_tokens(t_token *head);

// shell_loop.c
void	initialize_shell(void);
void	handle_input(char *input);
void	run_shell_loop(void);

// utils.c
int		ft_isspace(int c);
char	*ft_strndup(const char *s, size_t n);
void	error_message(const char *error);

#endif

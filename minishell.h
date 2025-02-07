/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/07 19:23:22 by mmravec          ###   ########.fr       */
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

// lexer.c
t_token	*lexer(const char *input);

// utils.c
int		ft_isspace(int c);
char	*ft_strndup(const char *s, size_t n);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:52:11 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/29 22:07:03 by mmravec          ###   ########.fr       */
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
# include <ctype.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>

# include "utils.h"
# include "env.h"
# include "token.h"
# include "parser.h"
# include "builtins.h"
# include "execution.h"
# include "history.h"
# include "lexer.h"
# include "signals.h"
# include "shell_loop.h"
# include "redirection.h"
# include "fd.h"

# define HISTORY_FILE ".history"

// Shell state structure
typedef struct s_shell
{
	t_env	*env;
	int		last_exit_code;
}	t_shell;

// global variable
extern int	g_signal_received;

// file_manipulation.c
int			open_file(char *name, char **file_content, int oflag);

// ft_strtok.c
char		*ft_strtok(char *s1, const char *s2);

// main.c
void		print_tokens(t_token *tokens);

#endif

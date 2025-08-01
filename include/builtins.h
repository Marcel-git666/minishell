/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:40:20 by marcel            #+#    #+#             */
/*   Updated: 2025/06/04 11:43:26 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include <dirent.h>
# include "env.h"
# include "parser.h"

// Forward declarations
struct						s_shell;
typedef struct s_shell		t_shell;
struct						s_ast_node;
typedef struct s_ast_node	t_ast_node;
typedef struct s_fds		t_fds;

//	exit
void	builtin_exit(t_shell *shell, t_fds *fd_red, t_ast_node *ast);

// pwd
void	builtin_pwd(t_shell *shell);

// cd
void	builtin_cd(t_ast_node *root, t_shell *shell);
char	*get_pwd(void);
void	previous_rep(t_shell *shell, char *cwd);
int		path(t_ast_node *root, char *cwd, t_shell *shell);
int		absolute_path(t_ast_node *root, int i, t_shell *shell);
int		only_cd(t_ast_node *root, t_shell *shell, char *cwd);

// export
void	builtin_export(t_ast_node *root, t_shell *shell);
int		check_parts(char **parts, int *i);

// unset
void	builtin_unset(t_ast_node *root, t_shell *shell);

// echo
void	builtin_echo(t_ast_node *root, t_shell *shell);

#endif

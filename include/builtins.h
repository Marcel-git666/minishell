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
struct s_shell;
typedef struct s_shell t_shell;
struct s_ast_node;
typedef struct s_ast_node t_ast_node;

//	exit
void	builtin_exit(t_shell *shell);

// pwd
void	builtin_pwd(t_shell *shell);

// cd
void	builtin_cd(t_ast_node *root, t_shell *shell);
char	*get_pwd(void);
void	path(t_ast_node *root, t_env *env, char *cwd);
void	previous_rep(t_env *env, char *cwd);
int		absolute_path(t_ast_node *root, int i, int j);

// export
void	builtin_export(t_ast_node *root, t_shell *shell);

// unset
void	builtin_unset(t_ast_node *root, t_shell *shell);

// echo
void	builtin_echo(t_ast_node *root, t_shell *shell);

#endif

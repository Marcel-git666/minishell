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
# include <limits.h>

//	exit
void	builtin_exit(void);

// pwd
void	builtin_pwd(void);

// cd
void	builtin_cd(t_ast_node *root, t_env *env);
char	*get_pwd(void);
void	path(t_ast_node *root, t_env *env, char *cwd);
void	previous_rep(t_env *env, char *cwd);
int		absolute_path(t_ast_node *root, int i, int j);

// export
void	builtin_export(t_ast_node *root, t_env *env);

// unset
void	builtin_unset(t_ast_node *root, t_env *env);

// echo
void	builtin_echo(t_ast_node *root);

#endif

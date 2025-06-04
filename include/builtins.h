/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:40:20 by marcel            #+#    #+#             */
/*   Updated: 2025/06/03 19:47:19 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include <dirent.h>
# include "env.h"
# include "parser.h"

//	exit
void	builtin_exit(void);

// pwd
void	builtin_pwd(t_env *env);

// cd
void	builtin_cd(t_ast_node *root, t_env *env);
char	*get_pwd(void);
void	path(t_ast_node *root, t_env *env, char *cwd);
void	previous_rep(t_env *env, char *cwd);
int		absolute_path(t_ast_node *root, int i, int j);

// export
t_env	*find_smallest(t_env *env);

#endif

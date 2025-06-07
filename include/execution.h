/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:50:12 by marcel            #+#    #+#             */
/*   Updated: 2025/06/06 16:49:38 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef EXECUTION_H
# define EXECUTION_H

# include "env.h"

// Forward declarations 
struct s_ast_node;
typedef struct s_ast_node t_ast_node;
struct s_shell;
typedef struct s_shell t_shell;

// execution.c
void		execute_command(t_ast_node *ast_node, t_shell *shell, char **envp);
void		search_command(t_ast_node *ast, t_env *env, char **envp);

// builtin.c
void		builtin_pwd(void);
void		builtin_cd(t_ast_node *root, t_env *env);
void		builtin_export(t_ast_node *root, t_env *env);

#endif
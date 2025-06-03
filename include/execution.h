/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:50:12 by marcel            #+#    #+#             */
/*   Updated: 2025/06/01 12:14:42 by lformank         ###   ########.fr       */
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
void		execute_command(t_ast_node *ast_node, t_shell *shell);

// builtin.c
void		builtin_pwd(t_env *env);
void		builtin_cd(t_ast_node *root, t_env *env);
void		builtin_export(t_ast_node *root, t_env *env);

#endif
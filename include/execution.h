/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:50:12 by marcel            #+#    #+#             */
/*   Updated: 2025/07/10 17:39:58 by marcel           ###   ########.fr       */
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
typedef struct s_fds t_fds;

// execution.c
void	    execute_command(t_ast_node *ast_node, t_shell *shell, char **envp);
int		    search_command(char *expanded_cmd, t_ast_node *ast, t_env *env, char **envp);
char	    *full_path(char **paths, char *path, char *cmd);
void		save_old(t_fds *fd);

// builtin.c
void		builtin_pwd(t_shell *shell);
void		builtin_cd(t_ast_node *root, t_shell *shell);
void		builtin_export(t_ast_node *root, t_shell *shell);

#endif
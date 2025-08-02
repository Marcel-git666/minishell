/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:50:12 by marcel            #+#    #+#             */
/*   Updated: 2025/08/02 08:41:27 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "env.h"

// Forward declarations
struct						s_ast_node;
typedef struct s_ast_node	t_ast_node;
struct						s_shell;
typedef struct s_shell		t_shell;
typedef struct s_fds		t_fds;

// execution.c
void	execute_command(t_ast_node *ast_node, t_shell *shell, char **envp);
void	save_old(t_fds *fd);

// execution_command.c
void	handle_command(t_ast_node *ast_node, t_shell *shell,
			char **envp, char *expanded_cmd);

// execution_search.c  
int		search_command(char *expanded_cmd, t_ast_node *ast, t_env *env,
			char **envp);
int		fork_it(t_ast_node *ast, char *path, char **args, char **envp);

// execution_utils.c
char	*full_path(char **paths, char *path, char *cmd);
void	free_source(char **path, char *slash, char *final_path, char **args);
char	**prepare_args(char *cmd, t_ast_node *ast);
void	free_args(char **args);

#endif
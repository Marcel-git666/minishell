/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:46:06 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 10:46:49 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "minishell.h"

// pipes.c
void    execute_pipe(t_ast_node *pipe_node, t_shell *shell, char **envp);

// pipes_execution.c
void	execute_left_child(int *pipe_fd, t_ast_node *left_node,
			t_shell *shell, char **envp);
void	execute_right_child(int *pipe_fd, t_ast_node *right_node,
			t_shell *shell, char **envp);
void	handle_parent_process(int *pipe_fd, pid_t left_pid,
			pid_t right_pid, t_shell *shell);
            
#endif
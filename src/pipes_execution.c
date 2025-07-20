/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:42:33 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 10:45:30 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

/*
 * Executes left side of pipe (writes to pipe output)
 * Redirects stdout to pipe write end and executes command
 */
void	execute_left_child(int *pipe_fd, t_ast_node *left_node,
			t_shell *shell, char **envp)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	execute_command(left_node, shell, envp);
	exit(0);
}

/*
 * Executes right side of pipe (reads from pipe input)
 * Redirects stdin to pipe read end and executes command
 */
void	execute_right_child(int *pipe_fd, t_ast_node *right_node,
			t_shell *shell, char **envp)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	execute_command(right_node, shell, envp);
	exit(0);
}

/*
 * Handles parent process cleanup and wait for children
 * Sets shell exit code based on right side command result
 */
void	handle_parent_process(int *pipe_fd, pid_t left_pid,
			pid_t right_pid, t_shell *shell)
{
	int	status;

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_code = WEXITSTATUS(status);
	else
		shell->last_exit_code = 1;
}

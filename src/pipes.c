/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:39:40 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 10:48:00 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 
#include "pipes.h"

/*
 * Creates pipe and forks left child process
 * Returns left child PID or -1 on error
 */
static pid_t	create_left_process(int *pipe_fd, t_ast_node *pipe_node,
				t_shell *shell, char **envp)
{
	pid_t	left_pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	left_pid = fork();
	if (left_pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (left_pid == 0)
		execute_left_child(pipe_fd, pipe_node->u_content.s_pipe.left,
			shell, envp);
	return (left_pid);
}

/*
 * Forks right child process
 * Returns right child PID or -1 on error
 */
static pid_t	create_right_process(int *pipe_fd, t_ast_node *pipe_node,
				t_shell *shell, char **envp)
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (right_pid == 0)
		execute_right_child(pipe_fd, pipe_node->u_content.s_pipe.right,
			shell, envp);
	return (right_pid);
}

/*
 * Executes pipe command by creating two child processes
 * Left child writes to pipe, right child reads from pipe
 */
void	execute_pipe(t_ast_node *pipe_node, t_shell *shell, char **envp)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	left_pid = create_left_process(pipe_fd, pipe_node, shell, envp);
	if (left_pid == -1)
		return ;
	right_pid = create_right_process(pipe_fd, pipe_node, shell, envp);
	if (right_pid == -1)
		return ;
	handle_parent_process(pipe_fd, left_pid, right_pid, shell);
}

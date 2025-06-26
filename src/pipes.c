/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:39:40 by marcel            #+#    #+#             */
/*   Updated: 2025/06/26 23:04:57 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 
#include "pipes.h"

static void	execute_left_child(int *pipe_fd, t_ast_node *left_node,
				t_shell *shell, char **envp)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	execute_command(left_node, shell, envp);
	exit(0);
}

static void	execute_right_child(int *pipe_fd, t_ast_node *right_node,
				t_shell *shell, char **envp)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	execute_command(right_node, shell, envp);
	exit(0);
}

static void	handle_parent_process(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(NULL);
	wait(NULL);
}

void	execute_pipe(t_ast_node *pipe_node, t_shell *shell, char **envp)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return ;
	}
	left_pid = fork();
	if (left_pid == 0)
		execute_left_child(pipe_fd, pipe_node->u_content.pipe.left,
			shell, envp);
	right_pid = fork();
	if (right_pid == 0)
		execute_right_child(pipe_fd, pipe_node->u_content.pipe.right,
			shell, envp);
	handle_parent_process(pipe_fd);
}

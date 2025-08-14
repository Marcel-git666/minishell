/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:42:33 by marcel            #+#    #+#             */
/*   Updated: 2025/08/14 12:30:28 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

/*
 * Sets up redirections for child process in pipe execution
 * Returns 1 if stdout was redirected to file, 0 otherwise
 * Properly cleans up fd structure after setup to prevent memory leaks
 */
static int	setup_child_redirections(t_ast_node *node, t_shell *shell)
{
	t_fds	*fd_red;
	int		stdout_redirected;

	fd_red = set_fd();
	if (!fd_red)
		exit(1);
	stdout_redirected = 0;
	if (process_heredocs(node, shell, fd_red) == -1)
	{
		reset_fd(fd_red);
		exit(130);
	}
	if (handle_redirections(node, fd_red, shell) == -1)
	{
		reset_fd(fd_red);
		exit(1);
	}
	if (fd_red->out_new != -1)
		stdout_redirected = 1;
	if (fd_red->temp)
	{
		free(fd_red->temp);
		fd_red->temp = NULL;
	}
	free(fd_red);
	return (stdout_redirected);
}

/*
 * Executes left child in pipe with proper redirection handling
 * Sets up stdout redirection and connects to pipe if needed
 */
void	execute_left_child(int *pipe_fd, t_ast_node *left_node,
			t_shell *shell, char **envp)
{
	int	stdout_redirected;
	int	exit_code;

	stdout_redirected = setup_child_redirections(left_node, shell);
	close(pipe_fd[0]);
	if (!stdout_redirected)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exit_code = execute_command(left_node, shell, envp);
	cleanup_resources(shell, NULL, NULL);
	exit(exit_code);
}

/*
 * Executes right child in pipe
 * Connects stdin to pipe and executes command
 */
void	execute_right_child(int *pipe_fd, t_ast_node *right_node,
			t_shell *shell, char **envp)
{
	int	exit_code;

	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exit_code = execute_command(right_node, shell, envp);
	cleanup_resources(shell, NULL, NULL);
	exit(exit_code);
}

/*
 * Handles parent process after forking pipe children
 * Waits for both children and sets shell exit code based on right child
 */
void	handle_parent_process(int *pipe_fd, pid_t left_pid,
			pid_t right_pid, t_shell *shell)
{
	int	left_status;
	int	right_status;

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status))
		shell->last_exit_code = WEXITSTATUS(right_status);
	else if (WIFSIGNALED(right_status))
		shell->last_exit_code = 128 + WTERMSIG(right_status);
	else
		shell->last_exit_code = 1;
}

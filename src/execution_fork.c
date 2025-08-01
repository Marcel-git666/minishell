/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:20:37 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 18:59:58 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Handles signal termination status from child process
 * Returns appropriate exit code based on signal type
 * Prints quit message for SIGQUIT, newline for SIGINT
 */
static int	handle_signal_termination(int status)
{
	if (WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		return (130);
	}
	if (WTERMSIG(status) == SIGQUIT)
	{
		error_message("Quit: 3");
		return (131);
	}
	return (128 + WTERMSIG(status));
}

/*
 * Waits for child process and handles its exit status
 * Restores signal handlers and processes termination reasons
 * Returns exit code based on how child process terminated
 */
static int	wait_for_child(int pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
		return (handle_signal_termination(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}

static void	cleanup_child_process(char **args)
{
	if (args)
		free_args(args);

}

int	fork_it(char *path, char **args, char **envp)
{
	int	pid;

	if (access(path, F_OK) != 0)
		return (127);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(path, args, envp) == -1)
		{
			cleanup_child_process(args);
			perror("error: execve failed");
			exit(127);
		}
	}
	else if (pid > 0)
		return (wait_for_child(pid));
	return (-1);
}

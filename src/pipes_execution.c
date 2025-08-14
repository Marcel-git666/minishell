/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:42:33 by marcel            #+#    #+#             */
/*   Updated: 2025/08/14 08:06:36 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

/*
 * Zpracuje přesměrování pro daný uzel. Vrací 1, pokud byl stdout přesměrován, jinak 0.
 * Tato funkce je klíčová pro správné chování rour.
 */
static int	setup_child_redirections(t_ast_node *node, t_shell *shell)
{
	t_fds	*fd_red;
	int		stdout_redirected;

	fd_red = set_fd();
	if (!fd_red)
		exit(1); // Nemůžeme pokračovat bez fds
	stdout_redirected = 0;
	if (process_heredocs(node, shell, fd_red) == -1)
		exit(130); // Heredoc přerušen
	if (handle_redirections(node, fd_red, shell) == -1)
		exit(1); // Chyba přesměrování
	if (fd_red->out_new != -1)
		stdout_redirected = 1;
	// Důležité: fd_red se zde NEUVOLŇUJE, protože file descriptory musí zůstat otevřené
	// pro execve. Systém je zavře po skončení procesu.
	return (stdout_redirected);
}

void	execute_left_child(int *pipe_fd, t_ast_node *left_node,
			t_shell *shell, char **envp)
{
	int	stdout_redirected;
	int exit_code;

	// 1. Nejprve nastavíme přesměrování specifická pro tento příkaz
	stdout_redirected = setup_child_redirections(left_node, shell);
	
	close(pipe_fd[0]); // Zavřeme čtecí konec roury
	// 2. Pokud stdout NEBYL přesměrován do souboru, napojíme ho na rouru
	if (!stdout_redirected)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]); // Vždy zavřeme zapisovací konec
	exit_code = execute_command(left_node, shell, envp);
	exit(exit_code);
}

void	execute_right_child(int *pipe_fd, t_ast_node *right_node,
			t_shell *shell, char **envp)
{
	int exit_code;

	close(pipe_fd[1]); // Zavřeme zapisovací konec roury
	dup2(pipe_fd[0], STDIN_FILENO); // Napojíme stdin na rouru
	close(pipe_fd[0]);
	
	// Pravá strana roury může mít také svá vlastní přesměrování
	exit_code = execute_command(right_node, shell, envp);
	exit(exit_code);
}

// Funkce handle_parent_process zůstává stejná, jak jsme ji opravili minule.
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

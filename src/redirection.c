/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:53:23 by lformank          #+#    #+#             */
/*   Updated: 2025/06/04 14:38:34 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection.h"

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

void	redirection(t_ast_node *root, t_shell *shell, char **envp)
{
	int				saved_stdin;
	int				saved_stdout;
	t_redirection	*redir;
	t_ast_node		*child_command;

	saved_stdin = -1;
	saved_stdout = -1;
	if (!root || !root->u_content.redir.redir)
	{
		error_message("invalid redirection node");
		shell->last_exit_code = 1;
		return ;
	}
	redir = root->u_content.redir.redir;
	child_command = root->u_content.redir.child;
	if (setup_redirection(redir, &saved_stdin, &saved_stdout) < 0)
	{
		shell->last_exit_code = 1;
		restore_fds(saved_stdin, saved_stdout);
		return ;
	}
	if (child_command)
		execute_command(child_command, shell, envp);
	restore_fds(saved_stdin, saved_stdout);
}

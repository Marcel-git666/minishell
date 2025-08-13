/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:03:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 17:01:45 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "expansion.h"
#include "pipes.h"

/*
 * Handles redirection setup and processing for commands
 * Sets up file descriptors and processes redirection nodes
 * Updates shell exit code and traverses redirection chain
 */
static int	handle_redirections(t_ast_node *node, t_fds *fd_red,
	t_shell *shell)
{
	t_redirection	*current_redir;

	current_redir = node->u_content.cmd.redirections;
	while (current_redir)
	{
		if (fd(node, fd_red, current_redir, shell) == -1)
			return (-1);
		current_redir = current_redir->next;
	}
	if (fd_red->out_new != -1)
	{
		dup2(fd_red->out_new, STDOUT_FILENO);
		close(fd_red->out_new);
	}
	if (fd_red->in_new != -1)
	{
		dup2(fd_red->in_new, STDIN_FILENO);
		close(fd_red->in_new);
	}
	return (0);
}

/*
 * Processes variable assignment nodes
 * Sets environment variable with given name and value
 * Updates shell exit code to indicate success
 */
static void	handle_assignment(t_ast_node *ast_node, t_shell *shell)
{
	env_set(&shell->env, ast_node->u_content.s_assign.name,
		ast_node->u_content.s_assign.value);
	shell->last_exit_code = 0;
}

static void	execute_actual_command(t_ast_node *ast_node, t_shell *shell,
	char **envp, t_fds *fd_red)
{
	char	*expanded_cmd;

	// Pokud uzel neobsahuje příkaz (např. jen přesměrování `> file`), nic neděláme
	if (!ast_node->u_content.cmd.cmd)
	{
		shell->last_exit_code = 0;
		return ;
	}
	expanded_cmd = expand_variables(ast_node->u_content.cmd.cmd,
			shell->env, shell->last_exit_code,
			ast_node->u_content.cmd.cmd_token_type == TOKEN_ENV_VAR);
	if (!expanded_cmd)
	{
		shell->last_exit_code = 1;
		return ;
	}
	if (ft_strcmp(expanded_cmd, "exit") == 0)
	{
		free(expanded_cmd);
		builtin_exit(shell, fd_red, ast_node);
	}
	else
	{
		handle_command(ast_node, shell, envp, expanded_cmd);
		free(expanded_cmd);
	}
}

static int	process_heredocs(t_ast_node *node, t_shell *shell, t_fds *fds)
{
	t_redirection	*redir;

	if (!node || node->type != NODE_COMMAND)
		return (0);
	redir = node->u_content.cmd.redirections;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			if (heredoc(shell, redir, fds) == -1)
			{
				return (-1); // Chyba nebo přerušení (Ctrl+C)
			}
		}
		redir = redir->next;
	}
	return (0);
}

/*
 * Main execution dispatcher for AST nodes
 * Handles redirections, pipes, assignments and commands
 * Sets up file descriptors and manages execution flow
 */
void	execute_command(t_ast_node *ast_node, t_shell *shell, char **envp)
{
	t_fds	*fd_red;

	if (!ast_node)
		return ;
	fd_red = set_fd();
	if (!fd_red)
	{
		shell->last_exit_code = 1;
		return ;
	}
	fd_red->in_old = dup(STDIN_FILENO);
	fd_red->out_old = dup(STDOUT_FILENO);
	if (ast_node->type == NODE_PIPE)
		execute_pipe(ast_node, shell, envp);
	else if (ast_node->type == NODE_ASSIGNMENT)
		handle_assignment(ast_node, shell);
	else if (ast_node->type == NODE_COMMAND)
	{
		// 1. Zpracujeme heredocs
		if (process_heredocs(ast_node, shell, fd_red) == 0)
		{
			// 2. Zpracujeme ostatní přesměrování
			if (handle_redirections(ast_node, fd_red, shell) == 0)
				execute_actual_command(ast_node, shell, envp, fd_red);
			else
				shell->last_exit_code = 1;
		}
	}
	reset_fd(fd_red);
}

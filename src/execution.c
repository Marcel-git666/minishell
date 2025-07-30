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
static void	handle_redirection(t_ast_node **ast_node, t_fds *fd_red,
	t_shell *shell)
{
	if (redirection(*ast_node, fd_red) == -1)
	{
		shell->last_exit_code = 1;
		return ;
	}
	while ((*ast_node)->type == NODE_REDIR && *ast_node)
	{
		*ast_node = (*ast_node)->u_content.s_redir.child;
		shell->last_exit_code = 0;
	}
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

/*
 * Expands environment variables in command strings
 * Handles both regular commands and environment variable references
 * Returns expanded command string or NULL on failure/empty result
 */
static char	*expand_command(t_ast_node *ast_node, t_shell *shell)
{
	int		cmd_is_env_var;
	char	*expanded_cmd;

	if (ft_strcmp(ast_node->u_content.cmd.cmd, "$") == 0)
	{
		printf("minishell: $: command not found\n");
		shell->last_exit_code = 127;
		return (NULL);
	}
	cmd_is_env_var = (ast_node->u_content.cmd.cmd_token_type == TOKEN_ENV_VAR
			|| ast_node->u_content.cmd.cmd_token_type == TOKEN_EXIT_CODE);
	expanded_cmd = expand_variables(ast_node->u_content.cmd.cmd,
			shell->env, shell->last_exit_code, cmd_is_env_var);
	if (!expanded_cmd)
	{
		shell->last_exit_code = 1;
		return (NULL);
	}
	if (ft_strlen(expanded_cmd) == 0)
	{
		printf("minishell: : command not found\n");
		shell->last_exit_code = 127;
		free(expanded_cmd);
		return (NULL);
	}
	return (expanded_cmd);
}

/*
 * Handles command execution with special case for exit builtin
 * Expands command, handles exit specially due to shell termination
 * Delegates to handle_command for other commands
 */
static void	handle_command_execution(t_ast_node *ast_node, t_shell *shell,
		char **envp, t_fds *fd_red)
{
	char	*expanded_cmd;

	expanded_cmd = expand_command(ast_node, shell);
	if (!expanded_cmd)
	{
		return ;
	}
	if (ft_strcmp(expanded_cmd, "exit") == 0)
	{
		free(expanded_cmd);
		expanded_cmd = NULL;
		builtin_exit(shell, fd_red, ast_node);
	}
	else
	{
		handle_command(ast_node, shell, envp, expanded_cmd);
	}
	if (expanded_cmd)
		free(expanded_cmd);
}

/*
 * Main execution dispatcher for AST nodes
 * Handles redirections, pipes, assignments and commands
 * Sets up file descriptors and manages execution flow
 */
void	execute_command(t_ast_node *ast_node, t_shell *shell, char **envp)
{
	t_fds	*fd_red;

	fd_red = set_fd();
	if (!ast_node)
	{
		reset_fd(fd_red);
		return ;
	}
	if (ast_node->type == NODE_REDIR)
	{
		handle_redirection(&ast_node, fd_red, shell);
	}
	if (ast_node && ast_node->type == NODE_PIPE)
	{
		reset_fd(fd_red);
		execute_pipe(ast_node, shell, envp);
		return ;
	}
	if (ast_node->type == NODE_ASSIGNMENT)
	{
		handle_assignment(ast_node, shell);
	}
	else if (ast_node->type == NODE_COMMAND)
	{
		handle_command_execution(ast_node, shell, envp, fd_red);
	}
	reset_fd(fd_red);
}

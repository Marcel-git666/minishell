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

static void	handle_redirection(t_ast_node **ast_node, t_fds *fd_red,
	t_shell *shell)
{
	if (redirection(*ast_node, fd_red) == -1)
	{
		reset_fd(fd_red);
		shell->last_exit_code = 1;
		return ;
	}
	while ((*ast_node)->type == NODE_REDIR && *ast_node)
	{
		*ast_node = (*ast_node)->u_content.redir.child;
		shell->last_exit_code = 0;
	}
}

static void	handle_assignment(t_ast_node *ast_node, t_shell *shell)
{
	printf("DEBUG: Processing NODE_ASSIGNMENT\n");
	printf("DEBUG: Setting %s = %s\n",
		ast_node->u_content.assign.name, ast_node->u_content.assign.value);
	env_set(&shell->env, ast_node->u_content.assign.name,
		ast_node->u_content.assign.value);
	shell->last_exit_code = 0;
	printf("DEBUG: Assignment completed\n");
}

static char	*expand_command(t_ast_node *ast_node, t_shell *shell)
{
	int		cmd_is_env_var;
	char	*expanded_cmd;

	printf("DEBUG: Processing NODE_COMMAND\n");
	cmd_is_env_var = (ast_node->u_content.cmd.cmd_token_type == TOKEN_ENV_VAR
			|| ast_node->u_content.cmd.cmd_token_type == TOKEN_EXIT_CODE);
	printf("DEBUG: Original cmd: '%s', cmd_token_type: %d, is_env_var: %d\n",
		ast_node->u_content.cmd.cmd, ast_node->u_content.cmd.cmd_token_type,
		cmd_is_env_var);
	expanded_cmd = expand_variables(ast_node->u_content.cmd.cmd,
			shell->env, shell->last_exit_code, cmd_is_env_var);
	if (!expanded_cmd)
	{
		printf("DEBUG: Expansion failed\n");
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

static void	handle_command_execution(t_ast_node *ast_node, t_shell *shell,
		char **envp, t_fds *fd_red)
{
	char	*expanded_cmd;

	expanded_cmd = expand_command(ast_node, shell);
	if (!expanded_cmd)
		return ;
	if (ft_strcmp(expanded_cmd, "exit") == 0)
		builtin_exit(shell, fd_red, ast_node);
	else
		handle_command(ast_node, shell, envp, expanded_cmd);
	free(expanded_cmd);
}

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
		handle_redirection(&ast_node, fd_red, shell);
	if (ast_node && ast_node->type == NODE_PIPE)
	{
		reset_fd(fd_red);
		execute_pipe(ast_node, shell, envp);
		return ;
	}
	if (ast_node->type == NODE_ASSIGNMENT)
		handle_assignment(ast_node, shell);
	else if (ast_node->type == NODE_COMMAND)
		handle_command_execution(ast_node, shell, envp, fd_red);
	reset_fd(fd_red);
}

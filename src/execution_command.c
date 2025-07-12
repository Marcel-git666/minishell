/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 00:26:50 by marcel            #+#    #+#             */
/*   Updated: 2025/07/13 00:54:08 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "expansion.h"

static void	expand_command_args(t_ast_node *ast_node, t_shell *shell)
{
	char			*expanded_arg;
	int				i;
	int				is_env_var;
	t_token_type	token_type;

	i = -1;
	while (++i < ast_node->u_content.cmd.arg_count)
	{
		token_type = ast_node->u_content.cmd.arg_token_types[i];
		if (token_type == TOKEN_SINGLE_QUOTED)
			continue ;
		is_env_var = (token_type == TOKEN_ENV_VAR
				|| token_type == TOKEN_EXIT_CODE);
		expanded_arg = expand_variables(ast_node->u_content.cmd.args[i],
				shell->env, shell->last_exit_code, is_env_var);
		free(ast_node->u_content.cmd.args[i]);
		ast_node->u_content.cmd.args[i] = expanded_arg;
	}
}

static int	is_builtin_command(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	return (0);
}

static void	execute_builtin_cmd(char *cmd, t_ast_node *ast_node, t_shell *shell)
{
	if (ft_strcmp(cmd, "env") == 0)
		env_print(shell);
	else if (ft_strcmp(cmd, "pwd") == 0)
		builtin_pwd(shell);
	else if (ft_strcmp(cmd, "cd") == 0)
		builtin_cd(ast_node, shell);
	else if (ft_strcmp(cmd, "export") == 0)
		builtin_export(ast_node, shell);
	else if (ft_strcmp(cmd, "unset") == 0)
		builtin_unset(ast_node, shell);
	else if (ft_strcmp(cmd, "echo") == 0)
		builtin_echo(ast_node, shell);
}

void	handle_command(t_ast_node *ast_node, t_shell *shell,
		char **envp, char *expanded_cmd)
{
	int		exit_code;

	expand_command_args(ast_node, shell);
	if (is_builtin_command(expanded_cmd))
		execute_builtin_cmd(expanded_cmd, ast_node, shell);
	else
	{
		exit_code = search_command(expanded_cmd, ast_node, shell->env, envp);
		shell->last_exit_code = exit_code;
	}
}

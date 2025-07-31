/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 11:54:09 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "builtins.h"

/*
 * Prints current working directory using getcwd()
 * Sets exit code to 0 on success, 1 on failure
 */
void	builtin_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		shell->last_exit_code = 0;
	}
	else
	{
		perror("pwd");
		shell->last_exit_code = 1;
	}
}

/*
 * Removes environment variables specified as arguments
 * Requires at least one argument, unsets each variable from shell environment
 */
void	builtin_unset(t_ast_node *root, t_shell *shell)
{
	int	i;

	if (root->u_content.cmd.arg_count == 0)
	{
		error_message("unset: not enough arguments");
		shell->last_exit_code = 1;
		return ;
	}
	i = 0;
	while (i < root->u_content.cmd.arg_count)
	{
		env_unset(&shell->env, root->u_content.cmd.args[i]);
		i++;
	}
	shell->last_exit_code = 0;
}

/*
 * Removes environment variables specified as arguments
 * Requires at least one argument, unsets each variable from shell environment
 */
void	builtin_echo(t_ast_node *root, t_shell *shell)
{
	int		i;
	int		newline;

	newline = 1;
	if (root->u_content.cmd.arg_count > 0
		&& ft_strcmp(root->u_content.cmd.args[0], "-n") == 0)
	{
		newline = 0;
		i = 0;
	}
	else
		i = -1;
	while (++i < root->u_content.cmd.arg_count && root->u_content.cmd.args[i])
	{
		if (root->u_content.cmd.args[i])
		{
			printf("%s", root->u_content.cmd.args[i]);
			if (i < root->u_content.cmd.arg_count - 1)
				printf(" ");
		}
	}
	if (newline)
		printf("\n");
	shell->last_exit_code = 0;
}

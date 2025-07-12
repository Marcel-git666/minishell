/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:51:13 by marcel            #+#    #+#             */
/*   Updated: 2025/07/12 20:14:26 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "builtins.h"

static int	handle_oldpwd(t_ast_node *root, t_shell *shell, char *cwd)
{
	char	*oldpwd;

	if (root->u_content.cmd.arg_count == 1
		&& ft_strcmp(root->u_content.cmd.args[0], "-") == 0)
	{
		oldpwd = env_get(shell->env, "OLDPWD");
		if (!oldpwd)
		{
			error_message("cd: OLDPWD not set");
			shell->last_exit_code = 1;
			free(cwd);
			return (1);
		}
		env_set(&shell->env, "OLDPWD", cwd);
		if (chdir(oldpwd) == -1)
		{
			perror("cd");
			shell->last_exit_code = 1;
			free(cwd);
			return (1);
		}
		return (1);
	}
	return (0);
}

static void	update_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = malloc(PATH_MAX * sizeof(char));
	if (getcwd(cwd, PATH_MAX))
	{
		env_set(&shell->env, "PWD", cwd);
		shell->last_exit_code = 0;
	}
	else
		shell->last_exit_code = 1;
	free(cwd);
}

static void	handle_cd_logic(t_ast_node *root, t_shell *shell, char *cwd)
{
	if (root->u_content.cmd.arg_count
		&& ft_strncmp(root->u_content.cmd.args[0], "..", 2) == 0)
	{
		env_set(&shell->env, "OLDPWD", cwd);
		previous_rep(shell, cwd);
	}
	else
	{
		env_set(&shell->env, "OLDPWD", cwd);
		path(root, cwd, shell);
	}
}

void	builtin_cd(t_ast_node *root, t_shell *shell)
{
	char	*cwd;

	cwd = malloc(PATH_MAX * sizeof(char));
	getcwd(cwd, PATH_MAX);
	if (root->u_content.cmd.arg_count > 1)
	{
		error_message("cd: too many arguments");
		free(cwd);
		shell->last_exit_code = 1;
		return ;
	}
	if (handle_oldpwd(root, shell, cwd))
	{
		update_pwd(shell);
		return ;
	}
	handle_cd_logic(root, shell, cwd);
	free(cwd);
	update_pwd(shell);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:51:13 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 16:20:19 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "builtins.h"

/*
 * Creates copy of oldpwd and handles cd - logic
 * Returns 1 on success, -1 on failure  
 */
static int	execute_cd_minus(char *oldpwd, char *cwd, t_shell *shell)
{
	char	*oldpwd_copy;

	oldpwd_copy = ft_strdup(oldpwd);
	if (!oldpwd_copy)
	{
		shell->last_exit_code = 1;
		free(cwd);
		return (-1);
	}
	env_set(&shell->env, "OLDPWD", cwd);
	if (chdir(oldpwd_copy) == -1)
	{
		perror("cd");
		shell->last_exit_code = 1;
		free(cwd);
		free(oldpwd_copy);
		return (-1);
	}
	printf("%s\n", oldpwd_copy);
	free(oldpwd_copy);
	return (1);
}

/*
 * Handles 'cd -' command to change to previous directory
 * Uses OLDPWD environment variable to get previous directory
 * Returns 1 if handled (success or failure), 0 if not applicable
 */
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
		return (execute_cd_minus(oldpwd, cwd, shell));
	}
	return (0);
}

/*
 * Updates PWD environment variable with current working directory
 * Uses getcwd to get current directory and sets PWD variable
 * Sets appropriate exit code based on success or failure
 */
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

/*
 * Initializes current working directory for cd operations
 * Allocates buffer and gets current directory path
 * Returns allocated cwd string or NULL on failure
 */
static char	*init_cwd(t_shell *shell)
{
	char	*cwd;

	cwd = malloc(PATH_MAX * sizeof(char));
	if (!cwd)
	{
		shell->last_exit_code = 1;
		return (NULL);
	}
	if (!getcwd(cwd, PATH_MAX))
	{
		free(cwd);
		shell->last_exit_code = 1;
		return (NULL);
	}
	return (cwd);
}

/*
 * Main cd builtin implementation
 * Handles argument validation, special cases, and directory changes
 * Updates PWD and OLDPWD environment variables appropriately
 */
void	builtin_cd(t_ast_node *root, t_shell *shell)
{
	char	*cwd;

	cwd = init_cwd(shell);
	if (!cwd)
		return ;
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
	env_set(&shell->env, "OLDPWD", cwd);
	if (path(root, cwd, shell) == 0)
		update_pwd(shell);
	free(cwd);
}

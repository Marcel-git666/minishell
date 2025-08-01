/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:07:52 by lformank          #+#    #+#             */
/*   Updated: 2025/07/20 12:17:37 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

/*
 * Changes to root directory and updates OLDPWD
 * Helper function for parent directory navigation
 * Sets OLDPWD to current directory before changing to root
 */
static void	handle_root_directory(t_shell *shell, char *cwd)
{
	chdir("/");
	env_set(&shell->env, "OLDPWD", cwd);
}

/*
 * Navigates to parent directory using cd ..
 * Handles special cases like root directory and /home paths
 * Updates OLDPWD before changing directory
 */
void	previous_rep(t_shell *shell, char *cwd)
{
	int		i;
	char	*address;
	char	*parent_path;

	i = 0;
	address = ft_strrchr(cwd, '/');
	while (&cwd[i] != address && cwd[i])
		i++;
	if (i == 0 && ft_strlen(cwd) == 1)
	{
		handle_root_directory(shell, cwd);
		return ;
	}
	else if (i == 0 && ft_strncmp(cwd, "/home", 6) == 0)
	{
		handle_root_directory(shell, cwd);
		return ;
	}
	parent_path = ft_substr(cwd, 0, i);
	if (parent_path)
	{
		chdir(parent_path);
		free(parent_path);
	}
	env_set(&shell->env, "OLDPWD", cwd);
}

/*
 * Changes to HOME directory when cd is called without arguments
 * Searches for HOME environment variable and changes to that directory
 * Returns 0 if handled successfully, 1 on error or if HOME not set
 */
static int	change_to_home(t_shell *shell, char *cwd)
{
	t_env	*env_ptr;

	env_ptr = shell->env;
	while (env_ptr && ft_strncmp(env_ptr->key, "HOME", 5) != 0)
		env_ptr = env_ptr->next;
	if (!env_ptr || !env_ptr->value)
	{
		shell->last_exit_code = 1;
		return (1);
	}
	chdir(env_ptr->value);
	if (env_set(&shell->env, "OLDPWD", cwd) == 0)
		free(cwd);
	return (0);
}

/*
 * Handles cd command without arguments (change to HOME)
 * Checks if no arguments provided and calls change_to_home
 * Returns 0 if handled, 1 if arguments present
 */
int	only_cd(t_ast_node *root, t_shell *shell, char *cwd)
{
	if (root->u_content.cmd.arg_count == 0)
		return (change_to_home(shell, cwd));
	return (1);
}

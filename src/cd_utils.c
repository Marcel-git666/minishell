/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:52:54 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 17:53:25 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "expansion.h"

/*
 * Prints cd error message for invalid paths
 * Outputs error to stderr and sets shell exit code to 1
 * Helper function for consistent error handling in cd operations
 */
static void	print_cd_error(char *path, t_shell *shell)
{
	int	j;

	j = 0;
	error_message("cd: no such file or directory: ");
	while (path[j])
		write(2, &path[j++], 1);
	write(2, "\n", 1);
	shell->last_exit_code = 1;
}

/*
 * Handles absolute path changes for cd command
 * Validates path length and attempts directory change
 * Returns 1 on success, 0 on failure with error message
 */
int	absolute_path(t_ast_node *root, int i, t_shell *shell)
{
	char	*expanded_path;

	if (i == 0 && ft_strlen(root->u_content.cmd.args[0]) > 1)
	{
		expanded_path = expand_tilde(root->u_content.cmd.args[0], shell->env);
		if (!expanded_path)
		{
			shell->last_exit_code = 1;
			return (0);
		}
		if (chdir(expanded_path) == -1)
		{
			print_cd_error(expanded_path, shell);
			free(expanded_path);
			return (0);
		}
		free(expanded_path);
	}
	return (1);
}

/*
 * Changes directory to root (/)
 * Simple helper function for root directory navigation
 * Used when path resolves to root directory
 */
static void	handle_root_path(void)
{
	chdir("/");
}

/*
 * Main path handling function for cd command
 * Handles various path types including relative, absolute, and special cases
 * Processes path arguments and delegates to appropriate handlers
 */
void	path(t_ast_node *root, char *cwd, t_shell *shell)
{
	int		i;
	char	*address;
	char	*expanded_path;

	i = 0;
	if (only_cd(root, shell, cwd) == 0)
		return ;
	expanded_path = expand_tilde(root->u_content.cmd.args[0], shell->env);
	if (!expanded_path)
	{
		shell->last_exit_code = 1;
		return ;
	}
	address = ft_strrchr(expanded_path, '/');
	while (expanded_path[i] && &expanded_path[i] != address)
		i++;
	if (i == 0 && ft_strlen(expanded_path) == 1)
		handle_root_path();
	else if (chdir(expanded_path) == -1)
	{
		print_cd_error(expanded_path, shell);
		free(expanded_path);
		return ;
	}
	free(expanded_path);
}

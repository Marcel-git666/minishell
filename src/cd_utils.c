/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:52:54 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 12:20:11 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

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
	if (i == 0 && ft_strlen(root->u_content.cmd.args[0]) > 1)
	{
		if (chdir(root->u_content.cmd.args[0]) == -1)
		{
			print_cd_error(root->u_content.cmd.args[0], shell);
			return (0);
		}
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

	i = 0;
	if (only_cd(root, shell, cwd) == 0)
		return ;
	address = ft_strrchr(root->u_content.cmd.args[0], '/');
	while (root->u_content.cmd.args[0][i]
		&& &root->u_content.cmd.args[0][i] != address)
		i++;
	if (i == 0 && ft_strlen(root->u_content.cmd.args[0]) == 1)
		handle_root_path();
	else if (absolute_path(root, i, shell) == 0)
		return ;
	else if (chdir(root->u_content.cmd.args[0]) == -1)
	{
		print_cd_error(root->u_content.cmd.args[0], shell);
		return ;
	}
}

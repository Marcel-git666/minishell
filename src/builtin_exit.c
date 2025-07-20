/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:45:48 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 12:19:07 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "builtins.h"

/*
 * Frees entire environment linked list and all allocated memory
 * Iterates through environment nodes freeing key, value and node
 * Sets shell->env to NULL after cleanup to prevent double free
 */
static void	free_env_list(t_shell *shell)
{
	t_env	*current;
	t_env	*next;

	current = shell->env;
	if (shell->env)
	{
		while (current)
		{
			next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = next;
		}
		shell->env = NULL;
	}
}

/*
 * Implements exit builtin command with complete cleanup
 * Frees all allocated memory including environment, AST, and file descriptors
 * Prints exit message and terminates shell with exit code 0
 */
void	builtin_exit(t_shell *shell, t_fds *fd, t_ast_node *ast)
{
	free_env_list(shell);
	free_ast(ast);
	if (fd->temp)
		free(fd->temp);
	free(fd);
	printf("Exiting minishell...\n");
	shell->last_exit_code = 0;
	exit(0);
}

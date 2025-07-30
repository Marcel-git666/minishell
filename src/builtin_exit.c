/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:45:48 by marcel            #+#    #+#             */
/*   Updated: 2025/07/30 12:18:02 by mmravec          ###   ########.fr       */
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
 * Checks if a string represents a valid number for exit code
 * Handles optional leading sign (+ or -)
 * Returns 1 if valid number, 0 if invalid
 * Empty string or non-numeric characters make it invalid
 */
int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
 * Implements exit builtin command with complete cleanup
 * Frees all allocated memory including environment, AST, and file descriptors
 * Prints exit message and terminates shell with exit code 0
 */
void	builtin_exit(t_shell *shell, t_ast_node *ast)
{
	int	exit_code;

	free_env_list(shell);
	if (ast->u_content.cmd.arg_count > 1)
	{
		error_message("exit: too many arguments");
		shell->last_exit_code = 1;
		return ;
	}
	if (ast->u_content.cmd.arg_count)
	{
		if (!is_valid_number(ast->u_content.cmd.args[0]))
		{
			error_message("exit: numeric argument required");
			shell->last_exit_code = 2;
			return ;
		}
		shell->last_exit_code = ft_atoi(ast->u_content.cmd.args[0]);
	}
	else
		shell->last_exit_code = 0;
	free_ast(ast);
	exit_code = shell->last_exit_code;
	free(shell);
	exit(exit_code);
}

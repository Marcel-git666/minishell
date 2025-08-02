/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:45:48 by marcel            #+#    #+#             */
/*   Updated: 2025/08/02 09:45:57 by lformank         ###   ########.fr       */
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
 * Cleans up all allocated resources before exit
 * Frees AST, file descriptors and shell structure
 */
void	cleanup_resources(t_shell *shell, t_fds *fd, t_ast_node *ast)
{
	if (ast)
		free_ast(ast);
	if (fd)
	{
		if (fd->temp)
			free(fd->temp);
		free(fd);
	}
	if (shell)
	{
		free_env_list(shell);
		free(shell);
	}
}

/*
 * Gets exit code from command arguments
 * Returns exit code or sets error message for invalid arguments
 */
static int	get_exit_code(t_ast_node *ast, t_shell *shell)
{
	if (ast->u_content.cmd.arg_count > 1)
	{
		error_message("exit: too many arguments");
		shell->last_exit_code = 1;
		return (-1);
	}
	if (ast->u_content.cmd.arg_count == 1)
	{
		if (!is_valid_number(ast->u_content.cmd.args[0]))
		{
			error_message("exit: numeric argument required");
			return (2);
		}
		return (ft_atoi(ast->u_content.cmd.args[0]));
	}
	return (shell->last_exit_code);
}

/*
 * Implements exit builtin command with complete cleanup
 * Validates arguments and terminates shell with appropriate exit code
 */
void	builtin_exit(t_shell *shell, t_fds *fd, t_ast_node *ast)
{
	int	exit_code;

	exit_code = get_exit_code(ast, shell);
	if (exit_code == -1)
		return ;
	free_env_list(shell);
	cleanup_resources(shell, fd, ast);
	exit(exit_code);
}

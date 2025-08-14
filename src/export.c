/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:26:09 by lformank          #+#    #+#             */
/*   Updated: 2025/08/14 07:21:46 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Processes export assignment (VAR=value) and adds to environment
 * Handles quoted values by removing surrounding quotes
 * Splits assignment string and sets environment variable
 */
static void	handle_export_assignment(char *assignment, t_shell *shell)
{
	char	*equals_pos;
	char	*key;
	char	*value;

	equals_pos = ft_strchr(assignment, '=');
	if (!equals_pos) // Případ `export VAR` bez hodnoty - nic se neděje
		return ;
	key = ft_strndup(assignment, equals_pos - assignment);
	if (!key)
		return ;
	value = ft_strdup(equals_pos + 1);
	if (!value)
	{
		free(key);
		return ;
	}
	env_set(&shell->env, key, value);
	free(key);
	free(value);
}

/*
 * Implements export builtin command with assignment and display modes
 * Handles VAR=value assignments, -p flag for display, and error cases
 * Sets appropriate exit codes based on operation success
 */
void	builtin_export(t_ast_node *root, t_shell *shell)
{
	int		i;
	char	*arg;
	char	*equals_pos;
	int		name_len;

	if (root->u_content.cmd.arg_count == 0)
	{
		env_print_sorted(shell); // `export` bez argumentů vypíše seřazené proměnné
		return ;
	}
	i = -1;
	shell->last_exit_code = 0; // Výchozí stav je úspěch
	while (++i < root->u_content.cmd.arg_count)
	{
		arg = root->u_content.cmd.args[i];
		equals_pos = ft_strchr(arg, '=');
		name_len = (equals_pos) ? (equals_pos - arg) : ft_strlen(arg);
		if (!is_valid_var_name(arg, name_len))
		{
			error_message("export: not a valid identifier");
			shell->last_exit_code = 1; // Nastavíme chybu
			continue ; // A pokračujeme na další argument
		}
		if (equals_pos)
			handle_export_assignment(arg, shell);
	}
}


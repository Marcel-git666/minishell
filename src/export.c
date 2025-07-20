/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:26:09 by lformank          #+#    #+#             */
/*   Updated: 2025/07/20 11:35:19 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Validates split assignment parts and handles cleanup on error
 * Checks for proper VAR=value format with both name and value present
 * Returns 0 on success, -1 on invalid format with cleanup
 */
int	check_parts(char **parts, int *i)
{
	if (!parts || !parts[0] || !parts[1])
	{
		if (parts)
		{
			(*i) = -1;
			while (parts[++(*i)])
				free(parts[*i]);
			free(parts);
		}
		return (-1);
	}
	return (0);
}

/*
 * Processes export assignment (VAR=value) and adds to environment
 * Handles quoted values by removing surrounding quotes
 * Splits assignment string and sets environment variable
 */
static void	handle_export_assignment(char *assignment, t_env **env)
{
	char	**parts;
	int		i;
	char	*clean_value;
	char	*temp;

	parts = ft_split(assignment, '=');
	if (check_parts(parts, &i) == -1)
		return ;
	clean_value = parts[1];
	if (clean_value[0] == '"' && clean_value[ft_strlen(clean_value) - 1] == '"')
	{
		temp = ft_substr(clean_value, 1, ft_strlen(clean_value) - 2);
		env_set(env, parts[0], temp);
		free(temp);
	}
	else
		env_set(env, parts[0], clean_value);
	i = -1;
	while (parts[++i])
		free(parts[i]);
	free(parts);
}

/*
 * Implements export builtin command with assignment and display modes
 * Handles VAR=value assignments, -p flag for display, and error cases
 * Sets appropriate exit codes based on operation success
 */
void	builtin_export(t_ast_node *root, t_shell *shell)
{
	if (root->u_content.cmd.arg_count == 1
		&& ft_strchr(root->u_content.cmd.args[0], '='))
	{
		handle_export_assignment(root->u_content.cmd.args[0], &shell->env);
		shell->last_exit_code = 0;
		return ;
	}
	if (root->u_content.cmd.arg_count > 1 || (root->u_content.cmd.arg_count == 1
			&& ft_strncmp(root->u_content.cmd.args[0], "-p", 2) != 0))
	{
		error_message("error: export: -l: invalid option\n"
			"export: usage: export or export -p");
		shell->last_exit_code = 1;
		return ;
	}
	if (root->u_content.cmd.arg_count == 0)
	{
		env_print_sorted(shell);
		return ;
	}
	shell->last_exit_code = 0;
}

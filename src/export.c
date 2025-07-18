/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:26:09 by lformank          #+#    #+#             */
/*   Updated: 2025/07/06 23:31:29 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parts(char **parts, int *i)
{
	if (!parts || !parts[0] || !parts[1])
	{
		// Free parts pokud jsou částečně alokovány
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

static void handle_export_assignment(char *assignment, t_env **env)
{
    char 	**parts;
    int 	i;
	char	*clean_value;

    parts = ft_split(assignment, '=');
	if (check_parts(parts, &i) == -1)
		return ;
	clean_value = parts[1];
	if (clean_value[0] == '"' && clean_value[ft_strlen(clean_value) - 1] == '"')
	{
    	char *temp = ft_substr(clean_value, 1, ft_strlen(clean_value) - 2);
    	env_set(env, parts[0], temp);
    	free(temp);
	}
	else
	    env_set(env, parts[0], clean_value);
    // Free parts
    i = -1;
    while (parts[++i])
        free(parts[i]);
    free(parts);
}

void	builtin_export(t_ast_node *root, t_shell *shell)
{
	if (root->u_content.cmd.arg_count == 1 &&
    	ft_strchr(root->u_content.cmd.args[0], '='))
	{
		handle_export_assignment(root->u_content.cmd.args[0], &shell->env);
		shell->last_exit_code = 0;
    	return ;
	}
	if (root->u_content.cmd.arg_count > 1  || (root->u_content.cmd.arg_count == 1
		&& ft_strncmp(root->u_content.cmd.args[0], "-p", 2) != 0))
	{
		error_message("error: export: -l: invalid option\nexport: usage: export \
or export -p");
		shell->last_exit_code = 1;
		return ;
	}
	t_env *current = shell->env;  // ← Lokální kopie pointeru
    while (current)
    {
        if (root->u_content.cmd.arg_count == 0)
            printf("declare -x %s=%s\n", current->key, current->value);
        current = current->next;  // ← Posuň lokální kopii
    }
	shell->last_exit_code = 0;
}

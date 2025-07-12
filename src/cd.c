/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:07:52 by lformank          #+#    #+#             */
/*   Updated: 2025/07/12 17:54:18 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

void	previous_rep(t_shell *shell, char *cwd)
{
	int		i;
	char	*address;

	i = 0;
	address = ft_strrchr(cwd, '/');
	while (&cwd[i] != address && cwd[i])
		i++;
	if (i == 0 && ft_strlen(cwd) == 1)
	{
		chdir("/");
		env_set(&shell->env, "OLDPWD", cwd);
		return ;
	}
	else if (i == 0 && ft_strncmp(cwd, "/home", 6) == 0)
	{
		chdir("/");
		env_set(&shell->env, "OLDPWD", cwd);
		return ;
	}
	chdir(ft_substr(cwd, 0, i));
	env_set(&shell->env, "OLDPWD", cwd);
	return ;
}

int	only_cd(t_ast_node *root, t_shell *shell, char *cwd)
{
	t_env	*env_ptr;

	if (root->u_content.cmd.arg_count == 0)
	{
		env_ptr = shell->env;
		while (env_ptr && ft_strncmp(env_ptr->key, "HOME", 5) != 0)
    		env_ptr = env_ptr->next;
		if (!env_ptr || !env_ptr->value)  // ✅ Kontrola před použitím
		{
    		shell->last_exit_code = 1;
    		return (1);  // Error - HOME not found
		}
		chdir(env_ptr->value);
		if (env_set(&shell->env, "OLDPWD", cwd) == 0)
			free(cwd);
		return (0);
	}
	return (1);
}

int	absolute_path(t_ast_node *root, int i, int j, t_shell *shell)
{
	if (i == 0 && ft_strlen(root->u_content.cmd.args[0]) > 1)
	{
		if (chdir(root->u_content.cmd.args[0]) == -1)
		{
			error_message("cd: no such file or directory: ");
			while (root->u_content.cmd.args[0][j])
				write(2, &root->u_content.cmd.args[0][j++], 1);
			write(2, "\n", 1);
			shell->last_exit_code = 1;
			return (0);
		}
	}
	return (1);
}

void	path(t_ast_node *root, char *cwd, t_shell *shell)
{
	int		i;
	int		j;
	char	*address;

	i = 0;
	j = 0;
	if (only_cd(root, shell, cwd) == 0)
		return ;
	address = ft_strrchr(root->u_content.cmd.args[0], '/');
	while (root->u_content.cmd.args[0][i]
		&& &root->u_content.cmd.args[0][i] != address)
		i++;
	if (i == 0 && ft_strlen(root->u_content.cmd.args[0]) == 1)
		chdir("/");
	else if (absolute_path(root, i, j, shell) == 0)
		return ;
	else if (chdir(root->u_content.cmd.args[0]) == -1)
	{
		error_message("cd: no such file or directory: ");
		while (root->u_content.cmd.args[0][j])
			write(2, &root->u_content.cmd.args[0][j++], 1);
		write(2, "\n", 1);
		shell->last_exit_code = 1;
		return ;
	}
}

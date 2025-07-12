/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:07:52 by lformank          #+#    #+#             */
/*   Updated: 2025/07/12 20:20:31 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

static void	handle_root_directory(t_shell *shell, char *cwd)
{
	chdir("/");
	env_set(&shell->env, "OLDPWD", cwd);
}

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
		handle_root_directory(shell, cwd);
		return ;
	}
	else if (i == 0 && ft_strncmp(cwd, "/home", 6) == 0)
	{
		handle_root_directory(shell, cwd);
		return ;
	}
	chdir(ft_substr(cwd, 0, i));
	env_set(&shell->env, "OLDPWD", cwd);
}

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

int	only_cd(t_ast_node *root, t_shell *shell, char *cwd)
{
	if (root->u_content.cmd.arg_count == 0)
		return (change_to_home(shell, cwd));
	return (1);
}

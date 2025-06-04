/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/03 20:41:32 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "env.h"
#include "builtins.h"

void	builtin_exit(void)
{
	printf("Exiting minishell...\n");
	exit(0);
}

void	builtin_pwd(t_env *env)
{
	char	*cwd;

	cwd = malloc(sizeof(char) * 100);
	if (!cwd)
		return ;
	getcwd(cwd, 99);
	printf("%s\n", env->value);
}

void	builtin_cd(t_ast_node *root, t_env *env)
{
	char	*cwd;

	cwd = get_pwd();
	if (root->u_content.cmd.arg_count > 1)
	{
		printf("~%s\n", cwd);
		return ;
	}
	if (root->u_content.cmd.arg_count
		&& ft_strncmp(root->u_content.cmd.args[0], "..", 2) == 0)
		previous_rep(env, cwd);	
	else
		path(root, env, cwd);
	cwd = get_pwd();
	env_set(&env, "PWD", cwd);
	free(cwd);
}

t_env	*find_smallest(t_env *env)
{
	t_env	*smallest;

	smallest = env;
	while (env)
	{
		if (ft_strcmp(env->key, smallest->key) < 0)
		{
			smallest = env;
			// printf("smallest: %s\n", smallest->key);
		}
		env = env->next;
	}
	return (smallest);
}

void	builtin_export(t_ast_node *root, t_env *env)
{
	t_env	*smallest;

	if (root->u_content.cmd.arg_count < 0 || root->u_content.cmd.arg_count > 1)
		return ;
	if (root->u_content.cmd.arg_count == 1
		&& ft_strncmp(root->u_content.cmd.args[1], "-p", 2) != 0)
	{
		error_message("-bash: export: -l: invalid option\nexport: usage: export\
			[-fn] [name[=value] ...] or export -p\n");
		return ;
	}
	smallest = find_smallest(env);
	// while (env)
	// {
		while (env)
		{
			if (ft_strncmp(env->key, smallest->key, ft_strlen(smallest->key)) == 0)
			{
				if (root->u_content.cmd.arg_count
					&& !ft_strncmp(root->u_content.cmd.args[0], "-p", 2))
				{
					printf("export: %s=%s\n", smallest->key, smallest->value);
				}
				else
				{
					printf("%s=%s\n", smallest->key, smallest->value);
				}
				smallest = env;
			}
			env = env->next;
		}
	// 	env = start;
	// }
}

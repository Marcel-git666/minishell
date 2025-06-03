/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/03 08:10:56 by lformank         ###   ########.fr       */
=======
/*   Updated: 2025/05/30 18:04:33 by lformank         ###   ########.fr       */
>>>>>>> 2cf40f32b73460ecf4a627e30dfd4069eb483053
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

void	builtin_export(t_ast_node *root, t_env *env)
{
	int		i;
	t_env	*first;
	t_env	*restart;

	i = -1;
	first = env;
	restart = env;
	while (env)
	{
		while (env && first->key && first->key[i] <= env->key[i])
		{
			while (first->key[i] == env->key[i])
				i++;
			if ((first->key[i] < env->key[i]))
				printf("first: %c and env: %c\n", first->key[i], env->key[i]);
			env = env->next;
		}
		
	}
}

t_env	*find_smallest(t_env *env, t_env *small)
{
	t_env	*smallest;

	smallest = env;
	while (env)
	{
		while (ft_strncmp(env->key, smallest->key, ft_strlen(env->key)) < 0)
			//&& ft_strncmp(env->key, small->key, ft_strlen(env->key) > 0))
		{
			if (!small)
				small = smallest;
			if (ft_strncmp(env->key, small->key, ft_strlen(env->key) > 0))
				smallest = env;
		}
		env = env->next;
	}
	return (smallest);
}

void	builtin_export(t_ast_node *root, t_env *env)
{
	t_env	*smallest;
	t_env	*small;
	t_env	*start;

	if (root->u_content.cmd.arg_count < 0 || root->u_content.cmd.arg_count > 1)
		return ;
	if (root->u_content.cmd.arg_count == 1
		&& ft_strncmp(root->u_content.cmd.args[1], "-p", 2) != 0)
	{
		error_message("-bash: export: -l: invalid option\nexport: usage: export\
			[-fn] [name[=value] ...] or export -p\n");
		return ;
	}
	small = 0;
	smallest = find_smallest(env, small);
	start = env;
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
			if (find_smallest(start, smallest) == 0)
				break ;
			smallest = find_smallest(start, smallest);
			env = env->next;
		}
	// 	env = start;
	// }
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/03 08:10:56 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

void	builtin_exit(void)
{
	printf("Exiting minishell...\n");
	exit(0);
}

void	builtin_pwd(t_env *env)
{
	while (env && ft_strncmp(env->key, "PWD", 4) != 0)
		env = env->next;
	printf("%s\n", env->value);
}

char	*previous_rep(t_env *env)
{
	int		i;
	char	*address;

	i = 0;
	//maybe I need to change the place of searching PWD in env to here too
	address = ft_strrchr(env->value, '/');
	while (&(env)->value[i] != address && &env->value[i])
	{
		printf("value[%d] = %c\n", i, env->value[i]);
		i++;
	}
	if (i == 0)
		return ("-1");
	chdir(ft_substr(env->value, 0, i));
	return (ft_substr(env->value, 0, i));
}

char	*only_slash(t_ast_node *root, t_env *env)
{
	int				i;
	char			*address;
	// DIR				*direct;
	// struct dirent	*dir;

	i = 0;
	if (root->u_content.cmd.arg_count == 0)
	{
		while (env && ft_strncmp(env->key, "HOME", 5) != 0)
			env = env->next;
		chdir(env->value);
		return (env->value);
	}
	address = ft_strrchr(root->u_content.cmd.args[0], '/');
	while (root->u_content.cmd.args[0][i]
		&& &root->u_content.cmd.args[0][i] != address)
		i++;
	if (i == 0 && ft_strlen(root->u_content.cmd.args[0]) == 1)
	{
		chdir("/");
		return ("/");
	}
	// else
	// {
	// 	direct = opendir("/src");
	// 	dir = readdir(direct);
	// 	while ((dir = readdir(direct)) != NULL)
	// 	{
	// 		printf("\t%s\n", dir->d_name);
	// 	}
	// }
	return ("/src");
}

// DO I need to validate the arguments before executing them? -> YES
void	builtin_cd(t_ast_node *root, t_env *env)
{
	t_env	*first_env;
	char	*oldpwd;
	char	*newpwd;

	first_env = env; // make function for setting oldpwd
	while (env && ft_strncmp(env->key, "PWD", 4) != 0)
		env = env->next;
	if (root->u_content.cmd.arg_count && root->u_content.cmd.arg_count > 1)
	{
		printf("~%s\n", env->value);
		return ;
	}
	oldpwd = ft_strndup(env->value, ft_strlen(env->value));
	if (root->u_content.cmd.arg_count && ft_strncmp(root->u_content.cmd.args[0], "..", 2) == 0)
	{
		newpwd = previous_rep(env);
		if (ft_strncmp(newpwd, "-1", 3) == 0)
			return ;
	}
	else if (root->u_content.cmd.arg_count == 0
		|| ft_strncmp(root->u_content.cmd.args[0], "/", 2) == 0) 
		newpwd = only_slash(root, first_env);
	env_set(&first_env, "PWD", newpwd);
	env_set(&first_env, "OLDPWD", oldpwd);
// printing
	env = first_env;
	while (env && ft_strncmp(env->key, "PWD", 4) != 0)
		env = env->next;
	printf("PWD: %s\n", env->value);
	env = first_env;
	while (env && ft_strncmp(env->key, "OLDPWD", 7) != 0)
		env = env->next;
	printf("OLDPWD: %s\n", env->value);
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

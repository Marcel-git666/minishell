/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/07 20:01:31 by mmravec          ###   ########.fr       */
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

char	*only_slash(t_ast_node *root)
{
	int		i;
	char	*address;

	i = -1;
	address = ft_strrchr(root->u_content.cmd.args[0], '/');
	while (root->u_content.cmd.args[0][++i] && root->u_content.cmd.args[0][i] != address)
		;
	printf("i: %d\n", i);
	if (i == 0)
	{
		chdir("/");
		return ("/");
	}
	// return (0);
}

// DO I need to validate the arguments before executing them?
void	builtin_cd(t_ast_node *root, t_env *env)
{
	t_env	*first_env;
	char	*oldpwd;
	char	*newpwd;

	first_env = env;
	while (env && ft_strncmp(env->key, "PWD", 4) != 0)
		env = env->next;
	oldpwd = ft_strndup(env->value, ft_strlen(env->value));
	if (ft_strncmp(root->u_content.cmd.args[0], "..", 2) == 0)
	{
		newpwd = previous_rep( env);
		if (ft_strncmp(newpwd, "-1", 3) == 0)
			return ;
	}
	else if (ft_strncmp(root->u_content.cmd.args[0], "/", 2) == 0) // i celá adresa uvnit5 funkce to rozdělím na root "/" a na absolutní adresu "/mnt/c/User/ladaf/..."
		newpwd = only_slash(root/*, env*/);
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

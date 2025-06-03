/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/30 18:04:33 by lformank         ###   ########.fr       */
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

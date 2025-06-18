/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 11:54:09 by lformank         ###   ########.fr       */
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

void	builtin_pwd(/*t_ast_node *ast*/)
{
    char	cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        perror("pwd");
}

void	builtin_cd(t_ast_node *root, t_env *env)
{
	char	*cwd;

	cwd = malloc(PATH_MAX * sizeof(char));
	getcwd(cwd, PATH_MAX);
	if (root->u_content.cmd.arg_count > 1)
	{
		printf("~%s\n", cwd);
		free(cwd);
		return ;
	}
	if (root->u_content.cmd.arg_count
		&& ft_strncmp(root->u_content.cmd.args[0], "..", 2) == 0)
		previous_rep(env, cwd);	
	else
		path(root, env, cwd);
	free(cwd);
	cwd = malloc(PATH_MAX * sizeof(char));
	getcwd(cwd, PATH_MAX);
	env_set(&env, "PWD", cwd);
	free(cwd);
}

void	builtin_export(t_ast_node *root, t_env *env)
{
	int		j;
	int		len;
	t_env	*start;

	j = -1;
	start = env;
	len = ft_envsize(env);
	if (root->u_content.cmd.arg_count > 1 || root->u_content.cmd.arg_count < 0
		|| (root->u_content.cmd.arg_count == 1
		&& ft_strncmp(root->u_content.cmd.args[0], "-p", 2) != 0))
	{
		error_message("-bash: export: -l: invalid option\nexport: usage: export\
or export -p");
		return ;
	}
	while (env && ++j < len)
	{
		while (env && j != env->order)
			env = env->next;
		if (env && root->u_content.cmd.arg_count == 1)
			printf("export %s=%s\n", env->key, env->value);
		else if (env)
			printf("%s=%s\n", env->key, env->value);
		env = start;
	}
}

void	builtin_unset(t_ast_node *root, t_env *env)
{
	int		i;
	t_env	*prev;
	t_env	*start;
	t_env	*to_free;

	i = -1;
	start = env;
	if (root->u_content.cmd.arg_count == 0)
	{
		error_message("unset: not enough arguments");
		return;
	}
	while (++i < root->u_content.cmd.arg_count && root->u_content.cmd.args[i])
	{
		while (env && ft_strncmp(root->u_content.cmd.args[i], env->key,
			ft_strlen(root->u_content.cmd.args[i])) != 0)
		{
			prev = env;
			env = env->next;
		}
		if (env && env->key && ft_strncmp(root->u_content.cmd.args[i], env->key,
			ft_strlen(root->u_content.cmd.args[i])) == 0)
		{
			to_free = env;
			env = env->next;
			prev->next = env;
		}
		env = start;
		if (to_free)
		{
			free(to_free->key);
			free(to_free->value);
			free(to_free);
			to_free = NULL;
		}
	}
}

void	builtin_echo(t_ast_node *root)
{
	int		i;
	int		newline = 1;

	if (root->u_content.cmd.arg_count > 0
		&& ft_strncmp(root->u_content.cmd.args[0], "-n", 2) == 0)
	{
		newline = 0;
		i = 0; // Start from the second argument
	}
	else
		i = -1; // Start from the first argument
	
	while (++i < root->u_content.cmd.arg_count && root->u_content.cmd.args[i])
    {
        if (root->u_content.cmd.args[i])
        {
            printf("%s", root->u_content.cmd.args[i]);
            // Add space only if not the last argument
            if (i < root->u_content.cmd.arg_count - 1)
                printf(" ");
        }
    }
	if (newline)
		printf("\n");
}

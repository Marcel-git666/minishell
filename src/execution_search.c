/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_search.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:32:54 by marcel            #+#    #+#             */
/*   Updated: 2025/07/13 00:53:17 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fork_it(char *path, char **args, char **envp)
{
	int	pid;
	int	status;

	if (access(path, F_OK) != 0)
		return (127);
	pid = fork();
	if (pid == 0)
	{
		if (execve(path, args, envp) == -1)
			perror("error: execve failed");
		exit(127);
	}
	else if (pid > 0)
	{
		wait(&status);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (-1);
	}
	return (-1);
}

static int	handle_direct_path(char *expanded_cmd, t_ast_node *ast, char **envp)
{
	char	**args;
	int		exit_code;

	if (access(expanded_cmd, F_OK) != 0)
		return (127);
	args = prepare_args(expanded_cmd, ast);
	if (!args)
		return (127);
	exit_code = fork_it(expanded_cmd, args, envp);
	free_args(args);
	return (exit_code);
}

static t_env	*find_path_env(t_env *env)
{
	while (env && ft_strncmp(env->key, "PATH", 5) != 0)
		env = env->next;
	return (env);
}

static int	search_in_path(char *expanded_cmd, t_ast_node *ast, t_env *env,
	char **envp)
{
	char	**paths;
	char	**args;
	char	*path;
	int		exit_code;

	env = find_path_env(env);
	if (!env)
		return (127);
	paths = ft_split(env->value, ':');
	args = prepare_args(expanded_cmd, ast);
	if (!paths || !args)
		return (127);
	path = full_path(paths, NULL, expanded_cmd);
	if (path)
	{
		exit_code = fork_it(path, args, envp);
		free_source(paths, NULL, path, args);
		return (exit_code);
	}
	free_source(paths, NULL, NULL, args);
	return (127);
}

int	search_command(char *expanded_cmd, t_ast_node *ast, t_env *env, char **envp)
{
	if (ft_strchr(expanded_cmd, '/'))
		return (handle_direct_path(expanded_cmd, ast, envp));
	else
		return (search_in_path(expanded_cmd, ast, env, envp));
}

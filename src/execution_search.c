/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_search.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:32:54 by marcel            #+#    #+#             */
/*   Updated: 2025/07/29 21:42:18 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Forks and executes a command with given path and arguments
 * Handles file accessibility check and process creation
 * Returns exit status of child process or error code (127)
 */
int	fork_it(char *path, char **args, char **envp)
{
	int	pid;
	int	status;

	if (access(path, F_OK) != 0)
		return (127);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, signal_handler_heredoc);
		if (execve(path, args, envp) == -1)
			perror("error: execve failed");
		exit(127);
	}
	else if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		setup_signals();
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (-1);
}

/*
 * Handles execution of commands with direct path (absolute or relative)
 * Checks file accessibility, prepares arguments and executes command
 * Returns exit code from command execution or 127 on failure
 */
static int	handle_direct_path(char *expanded_cmd, t_ast_node *ast, char **envp)
{
	char	**args;
	int		exit_code;

	if (access(expanded_cmd, F_OK) != 0)
	{
		print_command_not_found(expanded_cmd);
		return (127);
	}
	args = prepare_args(expanded_cmd, ast);
	if (!args)
		return (127);
	exit_code = fork_it(expanded_cmd, args, envp);
	free_args(args);
	return (exit_code);
}

/*
 * Finds PATH environment variable in the environment list
 * Iterates through environment variables looking for "PATH"
 * Returns pointer to PATH environment variable or NULL if not found
 */
static t_env	*find_path_env(t_env *env)
{
	while (env && ft_strcmp(env->key, "PATH") != 0)
		env = env->next;
	return (env);
}

/*
 * Searches for command in PATH directories and executes it
 * Splits PATH variable, searches each directory for executable
 * Returns exit code from execution or 127 if command not found
 */
static int	search_in_path(char *expanded_cmd, t_ast_node *ast, t_env *env,
	char **envp)
{
	char	**paths;
	char	**args;
	char	*path;
	int		exit_code;

	env = find_path_env(env);
	if (!env)
	{
		print_command_not_found(expanded_cmd);
		return (127);
	}
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
	print_command_not_found(expanded_cmd);
	return (127);
}

/*
 * Main command search function - handles both direct paths and PATH search
 * If command contains '/', treats as direct path, otherwise searches in PATH
 * Returns exit code from command execution
 */
int	search_command(char *expanded_cmd, t_ast_node *ast, t_env *env, char **envp)
{
	if (ft_strchr(expanded_cmd, '/'))
		return (handle_direct_path(expanded_cmd, ast, envp));
	else
		return (search_in_path(expanded_cmd, ast, env, envp));
}

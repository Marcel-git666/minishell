/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:03:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 17:01:45 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "env.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


char	*full_path(char **paths, char *path, t_ast_node *ast)
{
	char	*slash;
	int		i;

	i = -1;
	while (paths[++i])
	{
		slash = ft_strjoin(paths[i], "/");
		path = ft_strjoin(slash, ast->u_content.cmd.cmd);
		if (access(path, F_OK) == 0)
		{
			free(slash);
			return (path);
		}
		free(slash);	
		free(path);
	}
	return (NULL);
}

void	fork_it(char *path, char **args, char **envp)
{
	int	pid;

	pid = 0;
	if (access(path, F_OK) == 0)
	{
		pid = fork();
		if (pid == 0)
			execve(path, args, envp);
		else
			wait(&pid);
	}
}

void	free_source(char **path, char *slash, char *final_path, char **args)
{
	int	i;

	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
	free(final_path);
	i = -1;
	while (path[++i])
		free(path[i]);
	free(path);
	free(slash);
}

void	search_command(t_ast_node *ast, t_env *env, char **envp)
{
	char	**paths;
	char	*slash;
	char	*path;
	char	**args;
	int		i;

	i = -1;
	slash = NULL;
	path = NULL;
	while (env && ft_strncmp(env->key, "PATH", 5) != 0)
		env = env->next;
	paths = ft_split(env->value,':');
	args = ft_calloc((ast->u_content.cmd.arg_count + 2), sizeof(char *));
	args[0] = ft_strdup(ast->u_content.cmd.cmd);
	while (++i < ast->u_content.cmd.arg_count && ast->u_content.cmd.arg_count != 0)
		args[i + 1] = ft_strdup(ast->u_content.cmd.args[i]);
	path = full_path(paths, path, ast);
	fork_it(path, args, envp);
	free_source(paths, slash, path, args);
}

void	execute_command(t_ast_node *ast_node, t_shell *shell, char **envp)
{
	if (!ast_node)
		return ;
	if (ast_node->type == NODE_REDIR)
	{
		print_ast(ast_node, 0);
		print_ast(ast_node->u_content.redir.child, 0);
	}
	if (ast_node->type == NODE_COMMAND)
	{
		if (ft_strncmp(ast_node->u_content.cmd.cmd, "exit", 5) == 0)
			builtin_exit();
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "env", 4) == 0)
			env_print(shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "pwd", 4) == 0)
			builtin_pwd(shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "cd", 3) == 0)
			builtin_cd(ast_node, shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "export", 7) == 0)
			builtin_export(ast_node, shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "unset", 6) == 0)
			builtin_unset(ast_node, shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "echo", 5) == 0)
			builtin_echo(ast_node);
		else
		{
			search_command(ast_node, shell->env, envp);
			// For now, just print command info
			printf("Would execute: %s", ast_node->u_content.cmd.cmd);
			if (ast_node->u_content.cmd.arg_count > 0)
			{
				printf(" with args:");
				for (int i = 0; i < ast_node->u_content.cmd.arg_count; i++)
					printf(" %s", ast_node->u_content.cmd.args[i]);
			}
			printf("\n");
		}
	}
	return ;
}

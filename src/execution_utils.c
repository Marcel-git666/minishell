/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:33:05 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 11:52:12 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Constructs full path by searching through PATH directories
 * Concatenates each path with "/" and command name, checks accessibility
 * Returns allocated string with full path or NULL if not found
 */
char	*full_path(char **paths, char *path, char *cmd)
{
	char	*slash;
	int		i;

	i = -1;
	while (paths[++i])
	{
		slash = ft_strjoin(paths[i], "/");
		path = ft_strjoin(slash, cmd);
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

/*
 * Comprehensive cleanup function for memory allocated during path search
 * Frees argument array, final path, path directories and temporary slash
 * Handles all allocated memory to prevent leaks
 */
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

/*
 * Frees argument array and all its string elements
 * Iterates through array freeing each string, then frees array itself
 * Safely handles NULL arrays
 */
void	free_args(char **args)
{
	int		i;

	i = -1;
	if (!args)
		return ;
	while (args[++i])
		free(args[i]);
	free(args);
}

/*
 * Allocates memory for argument array with specified size
 * Uses ft_calloc to ensure all pointers are initialized to NULL
 * Returns allocated array or NULL on allocation failure
 */
static char	**allocate_args_array(int size)
{
	char	**args;

	args = ft_calloc(size, sizeof(char *));
	if (!args)
		return (NULL);
	return (args);
}

/*
 * Prepares argument array for command execution
 * Creates array with command name as first argument, followed by AST arguments
 * Returns NULL-terminated array suitable for execve or NULL on failure
 */
char	**prepare_args(char *cmd, t_ast_node *ast)
{
	char	**args;
	int		i;

	args = allocate_args_array(ast->u_content.cmd.arg_count + 2);
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cmd);
	if (!args[0])
	{
		free_args(args);
		return (NULL);
	}
	i = 0;
	while (i < ast->u_content.cmd.arg_count)
	{
		args[i + 1] = ft_strdup(ast->u_content.cmd.args[i]);
		if (!args[i + 1])
		{
			free_args(args);
			return (NULL);
		}
		i++;
	}
	return (args);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:33:05 by marcel            #+#    #+#             */
/*   Updated: 2025/08/14 09:04:03 by marcel           ###   ########.fr       */
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
 * Připraví pole argumentů pro execve, přičemž filtruje prázdné argumenty,
 * které mohly vzniknout po expanzi proměnných.
 */
/*
 * Připraví pole argumentů pro execve, přičemž filtruje prázdné argumenty,
 * které mohly vzniknout po expanzi proměnných.
 */
char	**prepare_args(char *cmd, t_ast_node *ast)
{
	char	**args;
	int		i;
	int		j;
	int		final_arg_count;

	// 1. Spočítáme, kolik argumentů není prázdných
	final_arg_count = 0;
	i = 0;
	while (i < ast->u_content.cmd.arg_count)
	{
		if (ast->u_content.cmd.args[i] && ast->u_content.cmd.args[i][0] != '\0')
			final_arg_count++;
		i++;
	}

	// 2. Alokujeme paměť jen pro platné argumenty (+ cmd + NULL)
	args = ft_calloc(final_arg_count + 2, sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cmd);
	if (!args[0])
	{
		free(args);
		return (NULL);
	}
	// 3. Zkopírujeme jen neprázdné argumenty
	i = 0;
	j = 1;
	while (i < ast->u_content.cmd.arg_count)
	{
		if (ast->u_content.cmd.args[i] && ast->u_content.cmd.args[i][0] != '\0')
		{
			args[j] = ft_strdup(ast->u_content.cmd.args[i]);
			j++;
		}
		i++;
	}
	return (args);
}

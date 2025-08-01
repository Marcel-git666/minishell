/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:52:54 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 10:05:14 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "expansion.h"

/*
 * Prints cd error message for invalid paths
 * Outputs error to stderr and sets shell exit code to 1
 * Helper function for consistent error handling in cd operations
 */
static void	print_cd_error(char *path, t_shell *shell)
{
	write(2, "minishell: cd: ", 15);
	write(2, path, ft_strlen(path));
	write(2, ": No such file or directory\n", 28);
	shell->last_exit_code = 1;
}

/*
 * Splits current directory into components and stores in array
 * Returns number of components added
 */
static int	split_current_dir(char *current_dir, char **components)
{
	char	*dir_copy;
	char	*token;
	int		count;

	count = 0;
	dir_copy = ft_strdup(current_dir);
	if (!dir_copy)
		return (0);
	token = ft_strtok(dir_copy, "/");
	while (token && count < 1023)
	{
		if (ft_strlen(token) > 0)
			components[count++] = ft_strdup(token);
		token = ft_strtok(NULL, "/");
	}
	free(dir_copy);
	return (count);
}

/*
 * Builds absolute path from component array
 * Returns allocated path string starting with /
 */
static char	*build_path_from_components(char **components, int count)
{
	char	*result;
	int		total_len;
	int		i;

	if (count == 0)
		return (ft_strdup("/"));
	total_len = 1;
	i = -1;
	while (++i < count)
		total_len += ft_strlen(components[i]) + 1;
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	result[0] = '/';
	result[1] = '\0';
	i = -1;
	while (++i < count)
	{
		ft_strlcat(result, components[i], total_len + 1);
		if (i < count - 1)
			ft_strlcat(result, "/", total_len + 1);
		free(components[i]);
	}
	return (result);
}

/*
 * Normalizes relative path by resolving .. and . components
 * Returns normalized absolute path or NULL on error
 */
static char	*normalize_relative_path(char *current_dir, char *path)
{
	char	**components;
	char	*path_copy;
	char	*token;
	char	*result;
	int		count;

	components = malloc(1024 * sizeof(char *));
	if (!components)
		return (NULL);
	count = 0;
	if (path[0] != '/')
		count = split_current_dir(current_dir, components);
	path_copy = ft_strdup(path);
	token = ft_strtok(path_copy, "/");
	while (token && count < 1023)
	{
		if (ft_strcmp(token, "..") == 0 && count > 0)
			free(components[--count]);
		else if (ft_strcmp(token, ".") != 0 && ft_strlen(token) > 0)
			components[count++] = ft_strdup(token);
		token = ft_strtok(NULL, "/");
	}
	result = build_path_from_components(components, count);
	return (free(path_copy), free(components), result);
}

/*
 * Main path handling function for cd command
 * Handles path normalization and directory change
 */
int	path(t_ast_node *root, char *cwd, t_shell *shell)
{
	char	*normalized_path;
	char	*expanded_path;

	if (only_cd(root, shell, cwd) == 0)
		return (0);
	expanded_path = expand_tilde(root->u_content.cmd.args[0], shell->env);
	if (!expanded_path)
	{
		shell->last_exit_code = 1;
		return (-1);
	}
	normalized_path = normalize_relative_path(cwd, expanded_path);
	if (!normalized_path || chdir(normalized_path) == -1)
	{
		print_cd_error(expanded_path, shell);
		free(expanded_path);
		free(normalized_path);
		return (-1);
	}
	free(expanded_path);
	free(normalized_path);
	return (0);
}

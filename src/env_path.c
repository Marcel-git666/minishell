/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:37:52 by marcel            #+#    #+#             */
/*   Updated: 2025/07/12 21:38:50 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_direct_path(const char *command)
{
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	return (NULL);
}

static void	free_paths_array(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

static char	*search_in_paths(const char *command, char **paths)
{
	char	*dir_with_slash;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		dir_with_slash = ft_strjoin(paths[i], "/");
		if (!dir_with_slash)
			return (NULL);
		full_path = ft_strjoin(dir_with_slash, command);
		free(dir_with_slash);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*path_resolve(const char *command, t_env *env)
{
	char	*path;
	char	**paths;
	char	*result;

	if (!command || !*command)
		return (NULL);
	result = check_direct_path(command);
	if (result)
		return (result);
	path = env_get(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(command, paths);
	free_paths_array(paths);
	return (result);
}

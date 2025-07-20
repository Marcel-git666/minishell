/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:08:26 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 12:02:33 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Counts number of nodes in environment linked list
 * Iterates through list incrementing counter for each node
 * Returns total count of environment variables
 */
int	ft_envsize(t_env *lst)
{
	int	len;

	len = 0;
	while (lst)
	{
		len++;
		lst = lst->next;
	}
	return (len);
}

/*
 * Creates environment string in KEY=VALUE format from env node
 * Concatenates key with "=" and value using ft_strjoin
 * Returns allocated string or NULL on allocation failure
 */
static char	*create_env_string(t_env *current)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(current->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, current->value);
	free(temp);
	return (result);
}

/*
 * Cleans up partially allocated environment array on failure
 * Frees all successfully allocated strings up to count
 * Prevents memory leaks during array creation failures
 */
static void	cleanup_env_array(char **env_array, int count)
{
	while (count > 0)
		free(env_array[--count]);
	free(env_array);
}

/*
 * Converts environment linked list to NULL-terminated string array
 * Creates array suitable for execve and other system functions
 * Returns allocated array or NULL on failure with proper cleanup
 */
char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*current;
	char	**env_array;
	int		i;

	count = ft_envsize(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			cleanup_env_array(env_array, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[count] = NULL;
	return (env_array);
}

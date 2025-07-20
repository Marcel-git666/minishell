/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:31:47 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 12:05:27 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Creates a new environment variable node with given key and value
 * Allocates memory for node and duplicates key/value strings
 * Returns allocated node or NULL on allocation failure with cleanup
 */
static t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

/*
 * Updates value of existing environment variable
 * Frees old value and replaces with new duplicated value
 * Returns 1 on success, 0 on allocation failure
 */
static int	update_existing_env(t_env *current, const char *value)
{
	free(current->value);
	current->value = ft_strdup(value);
	if (!current->value)
		return (0);
	return (1);
}

/*
 * Sets environment variable with given key-value pair
 * Updates existing variable or creates new one if not found
 * Returns 1 on success, 0 on failure or invalid parameters
 */
int	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	if (!env || !key || !value)
		return (0);
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (update_existing_env(current, value));
		current = current->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return (0);
	new_node->next = *env;
	*env = new_node;
	return (1);
}

/*
 * Retrieves value of environment variable by key
 * Searches through environment list for matching key
 * Returns pointer to value string or NULL if not found
 */
char	*env_get(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
 * Removes environment variable with given key from list
 * Searches and removes node, updating list linkage properly
 * Returns 1 if variable was found and removed, 0 otherwise
 */
int	env_unset(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !key)
		return (0);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

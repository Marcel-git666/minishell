/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:31:47 by marcel            #+#    #+#             */
/*   Updated: 2025/07/12 21:35:20 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*create_new_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

static int	update_existing_env(t_env *current, const char *value)
{
	free(current->value);
	current->value = ft_strdup(value);
	if (!current->value)
		return (0);
	return (1);
}

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
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return (0);
	new_node->next = *env;
	*env = new_node;
	return (1);
}

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

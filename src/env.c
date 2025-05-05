/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:24:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/05 01:22:04 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env   *env_init(char **envp)
{
    t_env	*env_list;
    int		i;
    char    *equals_pos;

    env_list = NULL;
    i = 0;
    while (envp[i])
    {
        t_env *new_node = malloc(sizeof(t_env));
        if (!new_node)
            return (NULL);
        equals_pos = ft_strchr(envp[i], '=');
        if (equals_pos)
        {
            // Split key and value at the equals sign
            new_node->key = ft_strndup(envp[i], equals_pos - envp[i]);
            new_node->value = ft_strdup(equals_pos + 1);
            
            if (!new_node->key || !new_node->value)
            {
                free(new_node->key);
                free(new_node);
                return (NULL); // Handle memory allocation failure
            }
            
            // Add to the list
            new_node->next = env_list;
            env_list = new_node;
        }
        else
        {
            // Handle case where there is no equals sign
            new_node->key = ft_strdup(envp[i]);
            new_node->value = ft_strdup("");
            if (!new_node->key || !new_node->value)
            {
                free(new_node->key);
                free(new_node);
                return (NULL);
            }
            new_node->next = env_list;
            env_list = new_node;
        }
        i++;
    }
    return (env_list);
}

void	env_free(t_env *env)
{
    t_env    *tmp;
    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}
void	env_print(t_env *env)
{
    while (env)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}
int	env_set(t_env **env, const char *key, const char *value)
{
    t_env	*current;
    if (!env || !key || !value)
        return (0); // Invalid input
    current = *env;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            if (!current->value)
                return (0);
            return (1); // Key found and value updated
        }
        current = current->next;
    }
    // If key not found, add new entry
    t_env	*new_node = malloc(sizeof(t_env));
    if (!new_node)
        return (0);
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    if (!current->key || !current->value)
    {
        free(current->key);
        free(current->value);
        free(current);
        return (0); // Memory allocation failed
    }
    new_node->next = *env;
    *env = new_node;
    return (1); // New key-value pair added
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

int    env_unset(t_env **env, const char *key)
{
    t_env	*current;
    t_env	*prev;

    if (!env || !key)
        return (0); // Invalid input
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
            return (1); // Key found and removed
        }
        prev = current;
        current = current->next;
    }
    return (0); // Key not found
}
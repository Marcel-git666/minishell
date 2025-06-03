/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:24:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/03 18:42:04 by lformank         ###   ########.fr       */
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
            new_node->order = 0;
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
    if (!new_node->key || !new_node->value)
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
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

char **env_to_array(t_env *env)
{
    int     count;
    t_env   *current;
    char    **env_array;
    char	*temp;
    char	*result;

    count = 0;
    current = env;
    while (current)
    {
        count++;
        current = current->next;
    }
    env_array = malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);
    current = env;
    count = 0;
    while (current)
    {
        temp = ft_strjoin(current->key, "=");
		if (!temp)
        {
            free(env_array[count]);
			while (count > 0)
			{
				free(env_array[--count]);
			}
			free(env_array);
			return (NULL);
        }
        result = ft_strjoin(temp, current->value);
		free(temp);
		if (!result)
		{
			free(env_array[count]);
			while (count > 0)
			{
				free(env_array[--count]);
			}
			free(env_array);
			return (NULL);
		}
        env_array[count++] = result;
        current = current->next;
    }
    env_array[count] = NULL;
    return (env_array);
}

char *path_resolve(const char *command, t_env *env)
{
    char *path;
    char *full_path;
    char **paths;
    char *dir_with_slash;
    int i;
    int j;

    if (!command || !*command)
        return (NULL);
    if (ft_strchr(command, '/')) {
        if (access(command, X_OK) == 0)
            return (ft_strdup(command));
        return (NULL);
    }
    path = env_get(env, "PATH");
    if (!path)
        return (NULL);
    paths = ft_split(path, ':');
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i]) {
        dir_with_slash = ft_strjoin(paths[i], "/");
        if (!dir_with_slash) {
            j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);
            return (NULL);
        }
        full_path = ft_strjoin(dir_with_slash, command);
        free(dir_with_slash);
        if (!full_path) {
            j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);
            return (NULL);
        }
        if (access(full_path, X_OK) == 0) {
            j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);
    return (NULL);
}

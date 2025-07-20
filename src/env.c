/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:24:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 11:13:16 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:24:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 11:13:16 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Sets key and value for environment node based on equals position
 * Handles both KEY=VALUE and KEY formats appropriately
 * Returns 1 on success, 0 on allocation failure
 */
static int	set_env_node_data(t_env *node, char *envp_line, char *equals_pos)
{
	if (equals_pos)
	{
		node->key = ft_strndup(envp_line, equals_pos - envp_line);
		node->value = ft_strdup(equals_pos + 1);
	}
	else
	{
		node->key = ft_strdup(envp_line);
		node->value = ft_strdup("");
	}
	if (!node->key || !node->value)
		return (0);
	return (1);
}

/*
 * Parses single environment string into key-value pair node
 * Splits on first '=' character, handles variables without values
 * Returns allocated env node or NULL on allocation failure with cleanup
 */
static t_env	*parse_envp_line(char *envp_line)
{
	t_env	*new_node;
	char	*equals_pos;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equals_pos = ft_strchr(envp_line, '=');
	if (!set_env_node_data(new_node, envp_line, equals_pos))
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

/*
 * Initializes environment linked list from system envp array
 * Parses each environment string and creates linked list
 * Returns head of environment list or NULL on allocation failure
 */
t_env	*env_init(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = parse_envp_line(envp[i]);
		if (!new_node)
		{
			env_free(env_list);
			return (NULL);
		}
		new_node->next = env_list;
		env_list = new_node;
		i++;
	}
	return (env_list);
}

/*
 * Frees entire environment linked list and all allocated memory
 * Iterates through list freeing key, value and node for each entry
 * Prevents memory leaks when cleaning up environment
 */
void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

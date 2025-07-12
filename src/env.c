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

static t_env	*parse_envp_line(char *envp_line)
{
	t_env	*new_node;
	char	*equals_pos;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equals_pos = ft_strchr(envp_line, '=');
	if (equals_pos)
	{
		new_node->key = ft_strndup(envp_line, equals_pos - envp_line);
		new_node->value = ft_strdup(equals_pos + 1);
	}
	else
	{
		new_node->key = ft_strdup(envp_line);
		new_node->value = ft_strdup("");
	}
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

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
			return (NULL);
		new_node->next = env_list;
		env_list = new_node;
		i++;
	}
	return (env_list);
}

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

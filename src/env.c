/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:24:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/03 18:25:46 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

void env_init(char **envp)
{
    t_env	*env_list;
    int		i;

    env_list = NULL;
    i = 0;
    while (envp[i])
    {
        t_env *new_node = malloc(sizeof(t_env));
        if (!new_node)
            return ;
        new_node->key = ft_strdup(envp[i]);
        new_node->value = ft_strdup(envp[i]);
        new_node->next = env_list;
        env_list = new_node;
        i++;
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:10:06 by marcel            #+#    #+#             */
/*   Updated: 2025/07/12 21:27:57 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_print(t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	shell->last_exit_code = 0;
}

void	sort_env_array(char **array, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(array[j], array[j + 1]) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i])
		free(env_array[i++]);
	free(env_array);
}

void	env_print_sorted(t_shell *shell)
{
	char	**env_array;
	int		count;
	int		i;

	env_array = env_to_array(shell->env);
	if (!env_array)
	{
		shell->last_exit_code = 1;
		return ;
	}
	count = ft_envsize(shell->env);
	sort_env_array(env_array, count);
	i = 0;
	while (env_array[i])
	{
		printf("declare -x %s\n", env_array[i]);
		i++;
	}
	free_env_array(env_array);
	shell->last_exit_code = 0;
}

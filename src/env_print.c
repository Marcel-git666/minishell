/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:10:06 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 12:01:32 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Prints all environment variables in key=value format
 * Iterates through environment list printing each variable
 * Sets shell exit code to 0 on successful completion
 */
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

/*
 * Sorts array of environment strings alphabetically using bubble sort
 * Compares adjacent strings and swaps if out of order
 * Modifies array in-place for use by export -p functionality
 */
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

/*
 * Frees array of environment strings and the array itself
 * Iterates through array freeing each string, then frees array
 * Prevents memory leaks when cleaning up sorted environment array
 */
static void	free_env_array(char **env_array)
{
	int	i;

	i = -1;
	while (env_array[++i])
		free(env_array[i]);
	free(env_array);
}

/*
 * Prints single environment line in export format
 * Splits on '=' and formats as 'declare -x KEY="VALUE"'
 * Handles variables without values by omitting quotes
 */
static void	print_sorted_env_line(char *env_line)
{
	char	*equals;

	equals = ft_strchr(env_line, '=');
	if (equals)
	{
		*equals = '\0';
		printf("declare -x %s=\"%s\"\n", env_line, equals + 1);
		*equals = '=';
	}
	else
		printf("declare -x %s\n", env_line);
}

/*
 * Prints environment variables in sorted export format
 * Creates array from environment, sorts it, prints in declare format
 * Sets appropriate exit code based on success or failure
 */
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
	i = -1;
	while (env_array[++i])
		print_sorted_env_line(env_array[i]);
	free_env_array(env_array);
	shell->last_exit_code = 0;
}

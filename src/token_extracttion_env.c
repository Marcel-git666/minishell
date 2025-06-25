/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extracttion_env.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:57:22 by marcel            #+#    #+#             */
/*   Updated: 2025/06/22 13:03:23 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_braced_var(const char *input, size_t *index)
{
	size_t	start;
	char	*var_name;

	(*index)++; // Skip {
	start = *index;
	while (input[*index] && input[*index] != '}')
		(*index)++;
	if (input[*index] != '}')
		return (error_message("syntax error: missing closing brace"), NULL);
	var_name = ft_strndup(input + start, *index - start);
	(*index)++; // Skip }
	return (var_name);
}

static char	*extract_simple_var(const char *input, size_t *index, size_t start)
{
	if (input[*index] == '?')
	{
		(*index)++; // Skip the ?
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(input[*index]) && input[*index] != '_')
		return (error_message(
				"syntax error: invalid environment variable name"), NULL);
	(*index)++;
	while (ft_isalnum(input[*index]) || input[*index] == '_')
		(*index)++;
	return (ft_strndup(input + start, *index - start));
}

char	*extract_env_var(const char *input, size_t *index)
{
	size_t	start;

	start = ++(*index);
	if (input[*index] == '{') // Handle ${VAR}
		return (extract_braced_var(input, index));
	return (extract_simple_var(input, index, start));
}

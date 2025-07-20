/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extracttion_env.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:57:22 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 11:13:28 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Extracts braced variable name from ${VAR} syntax
 * Validates closing brace and returns variable name
 * Returns NULL on syntax error (missing closing brace)
 */
static char	*extract_braced_var(const char *input, size_t *index)
{
	size_t	start;
	char	*var_name;

	(*index)++;
	start = *index;
	while (input[*index] && input[*index] != '}')
		(*index)++;
	if (input[*index] != '}')
		return (error_message("syntax error: missing closing brace"), NULL);
	var_name = ft_strndup(input + start, *index - start);
	(*index)++;
	return (var_name);
}

/*
 * Extracts simple variable name from $VAR syntax
 * Handles special case $? and validates variable naming rules
 * Returns variable name or NULL on invalid syntax
 */
static char	*extract_simple_var(const char *input, size_t *index, size_t start)
{
	if (input[*index] == '?')
	{
		(*index)++;
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

/*
 * Main environment variable extraction function
 * Handles both ${VAR} and $VAR syntax, delegates to appropriate helper
 * Returns variable name string or NULL on error
 */
char	*extract_env_var(const char *input, size_t *index)
{
	size_t	start;

	start = ++(*index);
	if (input[*index] == '{')
		return (extract_braced_var(input, index));
	return (extract_simple_var(input, index, start));
}

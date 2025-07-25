/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:46:01 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 12:33:42 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

/*
 * Handles memory cleanup on expansion error
 * Frees all allocated resources and returns NULL for error indication
 */
static char	*handle_expansion_error(char *result, char *var_name,
			char *var_value)
{
	free(result);
	free(var_name);
	free(var_value);
	return (NULL);
}

/*
 * Joins string parts during variable expansion
 * Concatenates result + var_value + rest with proper memory management
 * Returns joined string or NULL on allocation failure
 */
static char	*join_parts(char *result, char *var_value, char *rest)
{
	char	*temp;
	char	*final_result;

	temp = ft_strjoin(result, var_value);
	if (!temp)
		return (NULL);
	final_result = ft_strjoin(temp, rest);
	free(temp);
	return (final_result);
}

/*
 * Processes single variable expansion in string
 * Parses variable name, gets value, and replaces in result string
 * Returns updated string or NULL on error
 */
static char	*process_variable(char *result, char *dollar_pos,
			t_env *env, int exit_status)
{
	char	*var_name;
	char	*var_value;
	char	*final_result;
	int		var_len;

	var_len = parse_var_name(dollar_pos + 1, &var_name);
	if (var_len == 0)
		return (result);
	*dollar_pos = '\0';
	var_value = get_variable_value(var_name, env, exit_status);
	final_result = join_parts(result, var_value, dollar_pos + 1 + var_len);
	if (!final_result)
		return (handle_expansion_error(result, var_name, var_value));
	free(result);
	free(var_name);
	free(var_value);
	return (final_result);
}

/*
 * Main variable expansion function
 * Expands all $VAR, ${VAR}, and $? in input string
 * Handles both direct variable mode and string expansion mode
 * Returns expanded string or NULL on error
 */
char	*expand_variables(char *input, t_env *env, int exit_status,
		int is_env_var)
{
	char	*result;
	char	*dollar_pos;

	if (!input)
		return (NULL);
	if (is_env_var)
		return (get_variable_value(input, env, exit_status));
	result = ft_strdup(input);
	if (!result)
		return (NULL);
	dollar_pos = find_next_dollar(result);
	while (dollar_pos != NULL)
	{
		result = process_variable(result, dollar_pos, env, exit_status);
		if (!result)
			return (NULL);
		dollar_pos = find_next_dollar(result);
	}
	return (result);
}

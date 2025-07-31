/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:46:01 by marcel            #+#    #+#             */
/*   Updated: 2025/07/31 23:29:14 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

/*
 * Handles memory cleanup on expansion error
 * Frees all allocated resources and returns NULL for error indication
 */
// static char	*handle_expansion_error(char *result, char *var_name,
// 			char *var_value)
// {
// 	free(result);
// 	free(var_name);
// 	free(var_value);
// 	return (NULL);
// }

/*
 * Joins string parts during variable expansion
 * Concatenates result + var_value + rest with proper memory management
 * Returns joined string or NULL on allocation failure
 */
// static char	*join_parts(char *result, char *var_value, char *rest)
// {
// 	char	*temp;
// 	char	*final_result;

// 	temp = ft_strjoin(result, var_value);
// 	if (!temp)
// 		return (NULL);
// 	final_result = ft_strjoin(temp, rest);
// 	free(temp);
// 	return (final_result);
// }

/*
 * Processes single variable expansion in string
 * Parses variable name, gets value, and replaces in result string
 * Returns updated string or NULL on error
 */
// static char	*process_variable(char *result, char *dollar_pos,
// 			t_env *env, int exit_status)
// {
// 	char	*var_name;
// 	char	*var_value;
// 	char	*final_result;
// 	int		var_len;

// 	var_len = parse_var_name(dollar_pos + 1, &var_name);
// 	if (var_len == 0)
// 		return (result);
// 	*dollar_pos = '\0';
// 	var_value = get_variable_value(var_name, env, exit_status);
// 	final_result = join_parts(result, var_value, dollar_pos + 1 + var_len);
// 	if (!final_result)
// 		return (handle_expansion_error(result, var_name, var_value));
// 	free(result);
// 	free(var_name);
// 	free(var_value);
// 	return (final_result);
// }

/*
 * Main variable expansion function
 * Expands all $VAR, ${VAR}, and $? in input string
 * Handles both direct variable mode and string expansion mode
 * Returns expanded string or NULL on error
 */
static void	append_str_to_buffer(char *buffer, int *idx, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		buffer[*idx] = str[i];
		(*idx)++;
		i++;
	}
}

char	*expand_variables(char *input, t_env *env, int exit_status,
		int is_env_var)
// {
// 	char	*result;
// 	char	*dollar_pos;

// 	if (!input)
// 		return (NULL);
// 	if (is_env_var)
// 		return (get_variable_value(input, env, exit_status));
// 	result = ft_strdup(input);
// 	if (!result)
// 		return (NULL);
// 	dollar_pos = find_next_dollar(result);
// 	while (dollar_pos != NULL)
// 	{
// 		result = process_variable(result, dollar_pos, env, exit_status);
// 		if (!result)
// 			return (NULL);
// 		dollar_pos = find_next_dollar(result);
// 	}
// 	return (result);
// }
{
	char	*final_str;
	int		i;
	int		k;
	int		can_expand;

	if (!input)
		return (NULL);
	if (is_env_var)
		return (get_variable_value(input, env, exit_status));
	final_str = ft_calloc(4096, sizeof(char)); // Dostatečně velký buffer
	if (!final_str)
		return (NULL);
	i = 0;
	k = 0;
	can_expand = 1;
	while (input[i])
	{
		if (input[i] == 1) // Značka pro jednoduché uvozovky '
		{
			can_expand = !can_expand; // Přepne mód (ne)expandování
			i++;
			continue ;
		}
		if (input[i] == 2) // Značka pro dvojité uvozovky "
		{
			i++; // Jen přeskočíme, mód se nemění
			continue ;
		}
		if (input[i] == '$' && can_expand)
		{
			char *var_name;
			char *var_value;
			int var_len = parse_var_name(input + i + 1, &var_name);
			if (var_len > 0)
			{
				var_value = get_variable_value(var_name, env, exit_status);
				append_str_to_buffer(final_str, &k, var_value);
				free(var_name);
				free(var_value);
				i += var_len + 1;
				continue ;
			}
		}
		final_str[k++] = input[i++];
	}
	return (final_str);
}

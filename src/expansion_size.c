/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_size.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:23:15 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/01 16:36:54 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

/*
 * Calculates size needed for a single variable expansion
 * Returns length of expanded variable value
 */
static size_t	get_var_expansion_size(t_size_calc *calc)
{
	char	*var_name;
	char	*var_value;
	size_t	var_size;
	int		var_len;

	var_len = parse_var_name(&calc->input[calc->i], &var_name);
	if (var_len > 0)
	{
		var_value = get_variable_value(var_name, calc->env,
				calc->exit_status);
		var_size = ft_strlen(var_value);
		calc->i += var_len;
		free(var_name);
		free(var_value);
		return (var_size);
	}
	return (1);
}

/*
 * Processes quote character and updates quote state
 * Returns 1 (single character size)
 */
static size_t	process_quote_char(t_size_calc *calc)
{
	char	current_char;

	current_char = calc->input[calc->i - 1];
	if (current_char == '\'' && !calc->in_single_quotes)
		calc->in_single_quotes = 1;
	else if (current_char == '\'' && calc->in_single_quotes)
		calc->in_single_quotes = 0;
	return (1);
}

/*
 * Processes one character and returns its expanded size
 * Handles quotes, variables, and regular characters
 */
static size_t	process_single_char(t_size_calc *calc)
{
	if (calc->input[calc->i] == '\'')
	{
		calc->i++;
		return (process_quote_char(calc));
	}
	else if (calc->input[calc->i] == '$' && !calc->in_single_quotes)
	{
		calc->i++;
		return (get_var_expansion_size(calc));
	}
	else
	{
		calc->i++;
		return (1);
	}
}

/*
 * Calculates total buffer size needed for variable expansion
 * Pre-processes input to determine actual size after expansion
 */
size_t	calculate_expansion_size(char *input, t_env *env, int exit_status)
{
	t_size_calc	calc;

	calc.input = input;
	calc.env = env;
	calc.exit_status = exit_status;
	calc.i = 0;
	calc.in_single_quotes = 0;
	calc.total_size = 0;
	while (calc.input[calc.i])
		calc.total_size += process_single_char(&calc);
	return (calc.total_size + 1);
}

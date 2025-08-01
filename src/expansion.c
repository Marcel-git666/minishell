/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:46:01 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 16:28:05 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

/*
 * Initializes expansion state structure with input parameters
 * Allocates result buffer with calculated size and sets initial values
 * Returns 0 on success, -1 on allocation failure
 */
static int	init_expansion_state(t_expansion_state *state, char *input,
		t_env *env, int exit_status)
{
	size_t	buffer_size;

	state->input = input;
	buffer_size = calculate_expansion_size(input, env, exit_status);
	state->result = ft_calloc(buffer_size, sizeof(char));
	if (!state->result)
		return (-1);
	state->i = 0;
	state->j = 0;
	state->in_single_quotes = 0;
	state->in_double_quotes = 0;
	state->env = env;
	state->exit_status = exit_status;
	return (0);
}

/*
 * Handles variable expansion from $VAR syntax
 * Parses variable name and substitutes with environment value
 * Updates state indexes after processing variable
 */
static void	handle_variable_expansion(t_expansion_state *state)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	state->i++;
	var_len = parse_var_name(&state->input[state->i], &var_name);
	if (var_len > 0)
	{
		var_value = get_variable_value(var_name, state->env,
				state->exit_status);
		ft_strlcpy(&state->result[state->j], var_value,
			ft_strlen(var_value) + 1);
		state->j += ft_strlen(var_value);
		state->i += var_len;
		free(var_name);
		free(var_value);
	}
	else
		state->result[state->j++] = '$';
}

/*
 * Main expansion function for variables and quotes processing
 * Handles environment variable substitution and quote parsing
 * Returns allocated expanded string or NULL on failure
 */
char	*expand_variables(char *input, t_env *env, int exit_status,
	int is_env_var)
{
	t_expansion_state	state;
	char				c;

	if (is_env_var)
		return (get_variable_value(input, env, exit_status));
	if (!input || init_expansion_state(&state, input, env, exit_status) == -1)
		return (NULL);
	while (state.input[state.i])
	{
		c = state.input[state.i];
		if (c == '\'')
			process_single_quote(&state);
		else if (c == '"')
			process_double_quote(&state);
		else if (c == '$' && !state.in_single_quotes)
			handle_variable_expansion(&state);
		else
		{
			state.result[state.j++] = c;
			state.i++;
		}
	}
	state.result[state.j] = '\0';
	return (state.result);
}

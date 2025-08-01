/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:26:30 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 13:45:48 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"

/*
 * Processes single quote character during expansion
 * Handles quote state changes and literal quote copying
 * Updates state position and quote flags appropriately
 */
void	process_single_quote(t_expansion_state *state)
{
	state->i++;
	if (state->in_double_quotes)
		state->result[state->j++] = '\'';
	else
		state->in_single_quotes = !state->in_single_quotes;
}

/*
 * Processes double quote character during expansion
 * Handles quote state changes and literal quote copying
 * Updates state position and quote flags appropriately
 */
void	process_double_quote(t_expansion_state *state)
{
	state->i++;
	if (state->in_single_quotes)
		state->result[state->j++] = '"';
	else
		state->in_double_quotes = !state->in_double_quotes;
}

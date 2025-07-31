/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:46:01 by marcel            #+#    #+#             */
/*   Updated: 2025/07/31 23:53:34 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"


// Struktura pro uchování stavu při expanzi
typedef struct s_expansion_state
{
    char    *input;         // Původní řetězec z tokenu
    char    *result;        // Výsledný, expandovaný řetězec
    int     i;              // Index pro čtení z input
    int     j;              // Index pro zápis do result
    bool    in_single_quotes;
    bool    in_double_quotes;
    t_env   *env;
    int     exit_status;
} t_expansion_state;


// Pomocná funkce pro expanzi proměnné ($VAR)
static void	handle_variable_expansion(t_expansion_state *state)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

    // Posuneme se za znak '$'
	state->i++;
	var_len = parse_var_name(&state->input[state->i], &var_name);
	if (var_len > 0)
	{
		var_value = get_variable_value(var_name, state->env, state->exit_status);
		// Zkopírujeme hodnotu proměnné do výsledku
        ft_strlcpy(&state->result[state->j], var_value, ft_strlen(var_value) + 1);
        state->j += ft_strlen(var_value);
        state->i += var_len; // Posuneme vstupní index za jméno proměnné
		free(var_name);
		free(var_value);
	}
	else
	{
        // Pokud za '$' není platné jméno, bereme '$' doslovně
		state->result[state->j++] = '$';
	}
}

// Inicializace stavové struktury
static void init_expansion_state(t_expansion_state *state, char *input, t_env *env, int exit_status)
{
    state->input = input;
    // Alokujeme paměť pro nejhorší možný případ (velmi dlouhá proměnná)
    // V reálném projektu by se to dělalo dynamicky, ale pro teď to stačí.
    state->result = ft_calloc(ft_strlen(input) * 2 + 1, sizeof(char)); // Bezpečný odhad velikosti
    state->i = 0;
    state->j = 0;
    state->in_single_quotes = false;
    state->in_double_quotes = false;
    state->env = env;
    state->exit_status = exit_status;
}

/*
 * NOVÁ hlavní funkce pro expanzi.
 * Zpracovává uvozovky a proměnné.
 */
char *expand_variables(char *input, t_env *env, int exit_status, int is_env_var)
{
    t_expansion_state state;
    
    // Stará logika pro přímé proměnné může zůstat, pokud ji potřebuješ jinde.
    if (is_env_var)
		return (get_variable_value(input, env, exit_status));

    if (!input)
        return (NULL);

    init_expansion_state(&state, input, env, exit_status);
    if (!state.result)
        return (NULL); // Chyba alokace

    while (state.input[state.i])
    {
        char c = state.input[state.i];

        if (c == '\'')
        {
            if (!state.in_double_quotes)
                state.in_single_quotes = !state.in_single_quotes;
            else
                state.result[state.j++] = c; // Uvozovka uvnitř jiných uvozovek je brána doslovně
            state.i++;
        }
        else if (c == '"')
        {
            if (!state.in_single_quotes)
                state.in_double_quotes = !state.in_double_quotes;
            else
                state.result[state.j++] = c;
            state.i++;
        }
        else if (c == '$' && !state.in_single_quotes)
        {
            handle_variable_expansion(&state);
        }
        else
        {
            state.result[state.j++] = c;
            state.i++;
        }
    }
    state.result[state.j] = '\0'; // Ukončení výsledného řetězce
    return (state.result);
}
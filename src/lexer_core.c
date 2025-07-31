/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:08:44 by marcel            #+#    #+#             */
/*   Updated: 2025/07/31 23:23:04 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Skips whitespace characters in input string
 * Updates index pointer to next non-whitespace character
 */
void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

/*
 * Initializes lexer structure with default values
 * Sets up input pointer and resets all state flags
 */
void	init_lexer(t_lexer *lexer, const char *input)
{
	lexer->i = 0;
	lexer->tokens = NULL;
	lexer->input = input;
	lexer->is_file_expected = 0;
	lexer->is_delimiter_expected = 0;
}

/*
 * Processes current character in lexer loop
 * Handles special characters, assignments, and word extraction
 * Returns -1 on error, 0 to continue, 1 to break
 */
int process_current_char(t_lexer *lexer, int *is_first_word)
{
    char c = lexer->input[lexer->i];

    if (c == '\0')
        return (1); // Konec vstupu, ukonči smyčku.

    // Pokud je to operátor, zpracuj ho jako samostatný token.
    if (c == '|' || c == '<' || c == '>')
    {
        // Tuto funkci budeme muset také upravit, ale zatím to nechme tak.
        // Důležité je, že operátory řešíme odděleně.
        if (handle_special_tokens(lexer, is_first_word) == -1)
            return (-1); // Chyba
        return (0);    // Pokračuj ve smyčce
    }

    // Pokud to není operátor ani mezera, je to ZAČÁTEK SLOVA.
    // Zavoláme naši novou chytrou funkci, která "sežere" celé slovo
    // až do další mezery nebo operátoru.
    add_token_from_input(lexer, is_first_word);

    // add_token_from_input sama posune lexer->i, takže tady už nic neděláme.
    return (0); // Pokračuj ve smyčce
}

/*
 * Main lexer function that tokenizes input string
 * Processes input character by character and builds token list
 * Returns linked list of tokens or NULL on error
 */
t_token *lexer(const char *input)
{
    t_lexer lexer;
    int is_first_word;
    int result;

    init_lexer(&lexer, input);
    is_first_word = 1;
    while (lexer.input[lexer.i])
    {
        skip_whitespace(lexer.input, &(lexer.i));
        result = process_current_char(&lexer, &is_first_word);
        
        // ZACHYCENÍ CHYBY
        if (result == -1 || lexer.tokens == NULL)
        {
            // Pokud process_current_char nebo add_token_from_input selhaly,
            // lexer.tokens budou buď NULL, nebo je uvolníme.
            if (lexer.tokens)
                free_tokens(lexer.tokens);
            return (NULL); // Vracíme NULL, shell se nespustí.
        }

        if (result == 1) // Konec vstupu
            break;
    }
    return (lexer.tokens);
}

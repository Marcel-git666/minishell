/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/01 00:02:06 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Adds appropriate token based on lexer state and word content
 * Handles delimiter, file, command, assignment, and argument tokens
 * Now supports compound tokens for adjacent quotes/variables
 */
int add_token_from_input(t_lexer *lexer, int *is_first_word)
{
    char *word;

    // Místo všech starých extrakčních funkcí voláme jen jednu!
    word = extract_greedy_word(lexer->input, &(lexer->i));

    if (!word)
    {
        return (-1);
    }

    // Zbytek funkce zůstává podobný. Určí typ tokenu a přidá ho do seznamu.
    if (lexer->is_delimiter_expected)
    {
        add_token(&lexer->tokens, create_token(TOKEN_DELIMITER, word));
        lexer->is_delimiter_expected = 0;
    }
    else if (lexer->is_file_expected)
    {
        add_token(&lexer->tokens, create_token(TOKEN_FILE, word));
        lexer->is_file_expected = 0;
    }
    else if (*is_first_word)
    {
        // Tuto logiku můžeme později zjednodušit, ale prozatím je v pořádku.
        if (check_assignment(word))
            add_token(&(lexer->tokens), create_token(TOKEN_ASSIGNMENT, word));
        else
            add_token(&(lexer->tokens), create_token(TOKEN_CMD, word));
        *is_first_word = 0;
    }
    else
        add_token(&(lexer->tokens), create_token(TOKEN_ARG, word));

    free(word);
	return (0);
}

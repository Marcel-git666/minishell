/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/01 14:32:09 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Adds delimiter token and resets delimiter expectation flag
 */
static void	add_delimiter_token(t_lexer *lexer, char *word)
{
	add_token(&lexer->tokens, create_token(TOKEN_DELIMITER, word));
	lexer->is_delimiter_expected = 0;
}

/*
 * Adds file token and resets file expectation flag
 */
static void	add_file_token(t_lexer *lexer, char *word)
{
	add_token(&lexer->tokens, create_token(TOKEN_FILE, word));
	lexer->is_file_expected = 0;
}

/*
 * Adds command or assignment token based on word content
 * Resets first word flag after processing
 */
static void	add_first_word_token(t_lexer *lexer, char *word, int *is_first_word)
{
	if (check_assignment(word))
		add_token(&(lexer->tokens), create_token(TOKEN_ASSIGNMENT, word));
	else
		add_token(&(lexer->tokens), create_token(TOKEN_CMD, word));
	*is_first_word = 0;
}

/*
 * Determines appropriate token type and adds it to lexer
 * Handles different token types based on lexer state
 */
static void	add_appropriate_token(t_lexer *lexer, char *word,
	int *is_first_word)
{
	if (lexer->is_delimiter_expected)
		add_delimiter_token(lexer, word);
	else if (lexer->is_file_expected)
		add_file_token(lexer, word);
	else if (*is_first_word)
		add_first_word_token(lexer, word, is_first_word);
	else
		add_token(&(lexer->tokens), create_token(TOKEN_ARG, word));
}

/*
 * Extracts word from input and adds appropriate token to lexer
 * Handles delimiter, file, command, assignment, and argument tokens
 * Returns 0 on success, -1 on error
 */
int	add_token_from_input(t_lexer *lexer, int *is_first_word)
{
	char	*word;

	word = extract_greedy_word(lexer->input, &(lexer->i));
	if (!word)
		return (-1);
	add_appropriate_token(lexer, word, is_first_word);
	free(word);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/18 23:51:38 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

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
 * Adds appropriate token based on lexer state and word content
 * Handles delimiter, file, command, assignment, and argument tokens
 */
void	add_token_from_input(t_lexer *lexer, int *is_first_word)
{
	char	*word;

	word = extract_word(lexer->input, &(lexer->i),
			lexer->is_delimiter_expected);
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
		if (check_assignment(word))
			add_token(&(lexer->tokens), create_token(TOKEN_ASSIGNMENT, word));
		else
			add_token(&(lexer->tokens), create_token(TOKEN_CMD, word));
		*is_first_word = 0;
	}
	else
		add_token(&(lexer->tokens), create_token(TOKEN_ARG, word));
	free(word);
}

/*
 * Initializes lexer structure with default values
 */
static void	init_lexer(t_lexer *lexer, const char *input)
{
	lexer->i = 0;
	lexer->tokens = NULL;
	lexer->input = input;
	lexer->is_file_expected = 0;
	lexer->is_delimiter_expected = 0;
}

/*
 * Processes current character in lexer loop
 * Returns -1 on error, 0 to continue, 1 to break
 */
static int	process_current_char(t_lexer *lexer, int *is_first_word)
{
	if (lexer->input[lexer->i] == '\0')
		return (1);
	if (is_special_char(lexer->input[lexer->i])
		&& !lexer->is_delimiter_expected)
	{
		if (handle_special_tokens(lexer, is_first_word) == -1)
			return (-1);
		return (0);
	}
	if (handle_assignment_error(lexer) == -1)
		return (-1);
	if (is_word_char(lexer->input[lexer->i], lexer->is_delimiter_expected))
		add_token_from_input(lexer, is_first_word);
	else
		lexer->i++;
	return (0);
}

/*
 * Main lexer function that tokenizes input string
 * Returns linked list of tokens or NULL on error
 */
t_token	*lexer(const char *input)
{
	t_lexer	lexer;
	int		is_first_word;
	int		result;

	init_lexer(&lexer, input);
	is_first_word = 1;
	while (lexer.input[lexer.i])
	{
		skip_whitespace(lexer.input, &(lexer.i));
		result = process_current_char(&lexer, &is_first_word);
		if (result == -1)
			return (NULL);
		if (result == 1)
			break ;
	}
	return ((lexer.tokens));
}

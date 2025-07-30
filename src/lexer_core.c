/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:08:44 by marcel            #+#    #+#             */
/*   Updated: 2025/07/30 12:13:11 by mmravec          ###   ########.fr       */
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
int	process_current_char(t_lexer *lexer, int *is_first_word)
{
	if (lexer->input[lexer->i] == '\0')
		return (1);
	if (is_word_char(lexer->input[lexer->i], lexer->is_delimiter_expected)
		|| (is_special_char(lexer->input[lexer->i]) 
			&& (lexer->input[lexer->i] == '$' || lexer->input[lexer->i] == '\''
				|| lexer->input[lexer->i] == '"')))
	{
		add_token_from_input(lexer, is_first_word);
		return (0);
	}
	if (is_special_char(lexer->input[lexer->i])
		&& !lexer->is_delimiter_expected)
	{
		if (handle_special_tokens(lexer, is_first_word) == -1)
			return (-1);
		return (0);
	}
	if (handle_assignment_error(lexer) == -1)
		return (-1);
	lexer->i++;
	return (0);
}

/*
 * Main lexer function that tokenizes input string
 * Processes input character by character and builds token list
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

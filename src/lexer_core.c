/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:08:44 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 14:49:26 by marcel           ###   ########.fr       */
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
 * Checks if character is an operator that needs special handling
 * Returns 1 for operators, 0 for regular characters
 */
static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
 * Processes current character in lexer loop
 * Handles special characters, assignments, and word extraction
 * Returns -1 on error, 0 to continue, 1 to break
 */
int	process_current_char(t_lexer *lexer, int *is_first_word)
{
	char	c;

	c = lexer->input[lexer->i];
	if (c == '\0')
		return (1);
	if (is_operator_char(c))
	{
		if (handle_special_tokens(lexer, is_first_word) == -1)
			return (-1);
		return (0);
	}
	if (add_token_from_input(lexer, is_first_word) == -1)
		return (-1);
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
		if (result == -1 || lexer.tokens == NULL)
		{
			if (lexer.tokens)
				free_tokens(lexer.tokens);
			return (NULL);
		}
		if (result == 1)
			break ;
	}
	return (lexer.tokens);
}

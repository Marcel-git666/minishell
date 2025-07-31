/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/31 23:20:14 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Checks if current position should start compound token processing
 * Returns 1 if adjacent quotes/variables/text should be concatenated
 */
// static int	should_start_compound(t_lexer *lexer)
// {
// 	size_t	lookahead;

// 	lookahead = lexer->i;
// 	while (lexer->input[lookahead] && !ft_isspace(lexer->input[lookahead]))
// 	{
// 		if (is_special_char(lexer->input[lookahead])
// 			&& lexer->input[lookahead] != '$'
// 			&& lexer->input[lookahead] != '\''
// 			&& lexer->input[lookahead] != '"')
// 			break ;
// 		if ((lexer->input[lookahead] == '\'' || lexer->input[lookahead] == '"'
// 				|| lexer->input[lookahead] == '$') && lookahead > lexer->i)
// 			return (1);
// 		lookahead++;
// 	}
// 	return (0);
// }

/*
 * Extracts word based on compound or regular extraction
 * Returns allocated word string or NULL if extraction fails
 */
static char	*extract_word_token(t_lexer *lexer)
{
	// if (should_start_compound(lexer))
	// 	return (create_compound_token(lexer));
	// else
	// 	return (extract_word(lexer->input, &(lexer->i),
	// 			lexer->is_delimiter_expected));
	return (create_compound_token(lexer));
}

/*
 * Adds token based on lexer state (delimiter/file/command/assignment/arg)
 * Determines token type and adds to token list appropriately
 */
static void	add_typed_token(t_lexer *lexer, char *word, int *is_first_word)
{
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
}

/*
 * Adds appropriate token based on lexer state and word content
 * Handles delimiter, file, command, assignment, and argument tokens
 * Now supports compound tokens for adjacent quotes/variables
 */
void	add_token_from_input(t_lexer *lexer, int *is_first_word)
{
	char	*word;

	word = extract_word_token(lexer);
	if (!word)
		return ;
	add_typed_token(lexer, word, is_first_word);
	free(word);
}

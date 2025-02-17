/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/17 19:25:37 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

void	add_token_from_input(t_lexer *lexer, int *is_first_word)
{
	char	*word;

	word = extract_word(lexer->input, &(lexer->i));
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
		add_token(&(lexer->tokens), create_token(TOKEN_CMD, word));
		*is_first_word = 0;
	}
	else
		add_token(&(lexer->tokens), create_token(TOKEN_ARG, word));
	free(word);
}

t_token	*lexer(const char *input)
{
	t_lexer	lexer;
	int		is_first_word;

	lexer.i = 0;
	lexer.tokens = NULL;
	lexer.input = input;
	lexer.is_file_expected = 0;
	lexer.is_delimiter_expected = 0;
	is_first_word = 1;
	while (lexer.input[lexer.i])
	{
		skip_whitespace(lexer.input, &(lexer.i));
		if (lexer.input[lexer.i] == '\0')
			break ;
		if (is_special_char(lexer.input[lexer.i]))
		{
			if (handle_special_tokens(&lexer, &is_first_word) == -1)
				return (NULL);
			continue ;
		}
		if (ft_isalnum(lexer.input[lexer.i]) || lexer.input[lexer.i] == '-'
			|| lexer.input[lexer.i] == '/' || lexer.is_delimiter_expected
			|| lexer.input[lexer.i] == '.' || lexer.input[lexer.i] == '~')
			add_token_from_input(&lexer, &is_first_word);
		else
			lexer.i++;
	}
	return ((lexer.tokens));
}

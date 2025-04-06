/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/04/06 11:33:32 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

static int	is_valid_var_name(const char *word, int len)
{
	int		i;

	if (!len || (!ft_isalpha(word[0]) && word[0] != '_'))
		return (0);
	i = 0;
	while (i < len)
	{
		if (!ft_isalnum(word[i]) && word[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	check_assignment(char *word)
{
	int		i;
	char	*equal_pos;

	i = 0;
	// Skip any whitespace at start (shouldn't be any)
	while (word[i] && ft_isspace(word[i]))
		i++;
	if (!word[i])
		return (0);
	// Find first '='
	equal_pos = ft_strchr(word, '=');
	if (!equal_pos)
		return (0);
	// Check no space before =
	i = equal_pos - word - 1;
	if (i < 0 || ft_isspace(word[i]))
		return (0);
	// Check no space after =
	if (ft_isspace(*(equal_pos + 1)))
		return (0);
	// Check if variable name is valid
	return (is_valid_var_name(word, equal_pos - word));
}

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
		if (is_special_char(lexer.input[lexer.i]) && !lexer.is_delimiter_expected)
		{
			if (handle_special_tokens(&lexer, &is_first_word) == -1)
				return (NULL);
			continue ;
		}
		if (lexer.input[lexer.i] == '=' && (ft_isspace(lexer.input[lexer.i - 1])
			|| ft_isspace(lexer.input[lexer.i + 1])))
		{
			error_message("syntax error: spaces not allowed around '='. Use VAR=value instead");
			return (NULL);
		}
		if (ft_isalnum(lexer.input[lexer.i]) || lexer.input[lexer.i] == '-'
			|| lexer.input[lexer.i] == '/' || lexer.is_delimiter_expected
			|| lexer.input[lexer.i] == '.' || lexer.input[lexer.i] == '~'
			|| lexer.input[lexer.i] == '?' || lexer.input[lexer.i] == '*')
			add_token_from_input(&lexer, &is_first_word);
		else
			lexer.i++;
	}
	return ((lexer.tokens));
}

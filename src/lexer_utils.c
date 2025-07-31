/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:48:48 by marcel            #+#    #+#             */
/*   Updated: 2025/07/31 21:16:55 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Validates variable name according to shell naming rules
 * Must start with letter or underscore, contain only alphanumeric and underscore
 */
int	is_valid_var_name(const char *word, int len)
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

/*
 * Checks if word contains valid assignment syntax (VAR=value)
 * Validates no spaces around equals sign and proper variable naming
 */
int	check_assignment(char *word)
{
	int		i;
	char	*equal_pos;

	i = 0;
	while (word[i] && ft_isspace(word[i]))
		i++;
	if (!word[i])
		return (0);
	equal_pos = ft_strchr(word, '=');
	if (!equal_pos)
		return (0);
	i = equal_pos - word - 1;
	if (i < 0 || ft_isspace(word[i]))
		return (0);
	if (ft_isspace(*(equal_pos + 1)))
		return (0);
	return (is_valid_var_name(word, equal_pos - word));
}

/*
 * Checks if character is valid for word continuation
 * Includes alphanumeric, special shell characters, and path characters
 */
int	is_word_char(char c, int is_delimiter_expected)
{
	if (ft_isalnum(c) || c == '-' || c == '/' || c == '.')
		return (1);
	if (c == '~' || c == '?' || c == '*' || c == '+')
		return (1);
	if (is_delimiter_expected)
		return (1);
	return (0);
}

/*
 * Handles invalid assignment syntax error
 * Reports syntax error for spaces around equals sign
 */
int	handle_assignment_error(t_lexer *lexer)
{
	if (lexer->input[lexer->i] == '='
		&& (ft_isspace(lexer->input[lexer->i - 1])
			|| ft_isspace(lexer->input[lexer->i + 1])))
	{
		error_message("syntax error: spaces not allowed around '='. "
			"Use VAR=value instead");
		return (-1);
	}
	return (0);
}

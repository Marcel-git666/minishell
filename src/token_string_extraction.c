/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_string_extraction.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:40:18 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 11:12:08 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Extracts content from single-quoted string ('text')
 * No variable expansion or escape sequences within single quotes
 * Returns extracted string or NULL on unclosed quote error
 */
char	*extract_single_quoted_string(t_lexer *lexer)
{
	char	quote;
	char	*str;
	size_t	start;

	quote = lexer->input[(lexer->i)++];
	start = lexer->i;
	while (lexer->input[lexer->i] && lexer->input[lexer->i] != quote)
		(lexer->i)++;
	if (lexer->input[lexer->i] == '\0')
		return (error_message("syntax error: missing closing quote"), NULL);
	str = ft_strndup(lexer->input + start, lexer->i - start);
	(lexer->i)++;
	return (str);
}

/*
 * Extracts content from double-quoted string ("text")
 * Allows variable expansion and escape sequences within double quotes
 * Returns extracted string or NULL on unclosed quote error
 */
char	*extract_double_quoted_string(t_lexer *lexer)
{
	char	quote;
	size_t	start;
	char	*str;

	quote = lexer->input[(lexer->i)++];
	start = lexer->i;
	while (lexer->input[lexer->i] && lexer->input[lexer->i] != quote)
		(lexer->i)++;
	if (lexer->input[lexer->i] == '\0')
		return (error_message("syntax error: missing closing quote"), NULL);
	str = ft_strndup(lexer->input + start, lexer->i - start);
	(lexer->i)++;
	return (str);
}

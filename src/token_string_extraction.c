/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_string_extraction.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:40:18 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/14 17:30:54 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	(lexer->i)++; // Skip closing quote
	return (str);
}

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
	(lexer->i)++; // Skip closing quote
	return (str);
}

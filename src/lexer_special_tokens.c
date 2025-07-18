/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_special_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:59:03 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/19 00:01:38 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Handles pipe token validation and creation
 * Checks for syntax errors like consecutive pipes or misplaced pipes
 */
int	handle_pipe_token(t_lexer *lexer, int *is_first_word)
{
	int	j;
	int	k;

	if (*is_first_word)
		return (error_message("syntax error: `|` cannot start a command"), -1);
	j = lexer->i - 1;
	while (j >= 0 && ft_isspace(lexer->input[j]))
		j--;
	k = lexer->i + 1;
	while (lexer->input[k] && ft_isspace(lexer->input[k]))
		k++;
	if (lexer->input[k] == '|')
		return (error_message("syntax error: consecutive pipes not allowed"),
			-1);
	lexer->i++;
	while (lexer->input[lexer->i] && ft_isspace(lexer->input[lexer->i]))
		lexer->i++;
	if (j < 0 || !lexer->input[lexer->i])
		return (error_message("syntax error: `|` cannot start or end command"),
			-1);
	add_token(&(lexer->tokens), create_token(TOKEN_PIPE, "|"));
	*is_first_word = 1;
	return (0);
}

/*
 * Processes single quoted strings and creates appropriate token
 */
static int	handle_single_quote(t_lexer *lexer)
{
	char	*quoted;

	quoted = extract_single_quoted_string(lexer);
	if (!quoted)
		return (-1);
	if (ft_strlen(quoted) > 0)
		add_token(&(lexer->tokens), create_token(TOKEN_SINGLE_QUOTED, quoted));
	free(quoted);
	return (0);
}

/*
 * Processes double quoted strings and creates appropriate token
 */
static int	handle_double_quote(t_lexer *lexer)
{
	char	*quoted;

	quoted = extract_double_quoted_string(lexer);
	if (!quoted)
		return (-1);
	if (ft_strlen(quoted) > 0)
		add_token(&(lexer->tokens), create_token(TOKEN_DOUBLE_QUOTED, quoted));
	free(quoted);
	return (0);
}

/*
 * Handles both single and double quoted strings
 * Delegates to appropriate quote handler based on quote type
 */
int	handle_quote_token(t_lexer *lexer, int *is_first_word)
{
	int	result;

	if (lexer->input[lexer->i] == '\'')
		result = handle_single_quote(lexer);
	else
		result = handle_double_quote(lexer);
	if (*is_first_word)
		*is_first_word = 0;
	return (result);
}

/*
 * Extracts compound token from current position to end of word
 * Handles complex tokens that contain special characters
 */
char	*extract_compound_token(t_lexer *lexer, size_t start_pos)
{
	size_t	end_pos;
	char	*compound;

	end_pos = lexer->i;
	while (lexer->input[end_pos] && !ft_isspace(lexer->input[end_pos]))
	{
		if ((lexer->input[end_pos] == '|' || lexer->input[end_pos] == '<'
				|| lexer->input[end_pos] == '>'))
			break ;
		end_pos++;
	}
	compound = ft_strndup(lexer->input + start_pos, end_pos - start_pos);
	lexer->i = end_pos;
	return (compound);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_compound.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 12:35:07 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 13:06:49 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Checks if character starts a quote or variable expansion
 * Returns 1 if character begins a compound token element
 */
static int	is_compound_start(char c)
{
	return (c == '\'' || c == '"' || c == '$');
}

/*
 * Checks if current position should continue compound token
 * Returns 1 if adjacent content exists that should be concatenated
 */
static int	should_continue_compound(t_lexer *lexer)
{
	if (!lexer->input[lexer->i] || ft_isspace(lexer->input[lexer->i]))
		return (0);
	if (is_compound_start(lexer->input[lexer->i]))
		return (1);
	if (is_special_char(lexer->input[lexer->i])
		&& lexer->input[lexer->i] != '$')
		return (0);
	return (1);
}

/*
 * Processes single element of compound token (quote, variable, or text)
 * Updates lexer position and returns processed content
 */
static char	*process_compound_element(t_lexer *lexer)
{
	size_t	start;

	if (lexer->input[lexer->i] == '\'')
		return (extract_single_quoted_string(lexer));
	else if (lexer->input[lexer->i] == '"')
		return (extract_double_quoted_string(lexer));
	else if (lexer->input[lexer->i] == '$')
		return (extract_env_var(lexer->input, &lexer->i));
	else
	{
		start = lexer->i;
		while (lexer->input[lexer->i] && !ft_isspace(lexer->input[lexer->i])
			&& !is_compound_start(lexer->input[lexer->i])
			&& !is_special_char(lexer->input[lexer->i]))
			lexer->i++;
		return (ft_strndup(lexer->input + start, lexer->i - start));
	}
}

/*
 * Joins multiple elements into single compound token
 * Concatenates all parts with proper memory management
 */
static char	*join_compound_elements(char **elements, int count)
{
	char	*result;
	char	*temp;
	int		i;

	if (count == 0)
		return (ft_strdup(""));
	result = ft_strdup(elements[0]);
	i = 1;
	while (i < count && result)
	{
		temp = ft_strjoin(result, elements[i]);
		free(result);
		result = temp;
		i++;
	}
	return (result);
}

/*
 * Creates compound token from adjacent quoted/unquoted parts
 * Handles cases like hello'world' -> helloworld
 * Returns compound token value or NULL on error
 */
char	*create_compound_token(t_lexer *lexer)
{
	char	*elements[32];
	char	*result;
	int		count;
	int		i;

	count = 0;
	while (should_continue_compound(lexer) && count < 32)
	{
		elements[count] = process_compound_element(lexer);
		if (!elements[count])
			break ;
		count++;
	}
	result = join_compound_elements(elements, count);
	i = -1;
	while (++i < count)
		free(elements[i]);
	return (result);
}

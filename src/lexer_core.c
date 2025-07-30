/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:08:44 by marcel            #+#    #+#             */
/*   Updated: 2025/07/30 22:00:35 by marcel           ###   ########.fr       */
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

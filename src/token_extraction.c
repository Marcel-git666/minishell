/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:46:37 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/31 23:21:10 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Checks if character is a special shell character
 * Returns 1 for pipe, redirections, quotes, and variable expansion
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$'
		|| c == '\'' || c == '"');
}

/*
 * Handles quote state tracking and equals sign detection
 * Updates quote state and returns 1 if equals sign found
 */
static int	handle_quotes_and_special(const char *input, size_t *index,
				int *in_quotes, char *quote_char)
{
	int	found_equals;

	found_equals = 0;
	if ((input[*index] == '"' || input[*index] == '\'') && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_char = input[*index];
	}
	else if (input[*index] == *quote_char && *in_quotes)
		*in_quotes = 0;
	if (input[*index] == '=')
		found_equals = 1;
	return (found_equals);
}

/*
 * Extracts word tokens from input, handling quotes and special characters
 * Respects quote boundaries and stops at delimiters appropriately
 * Returns allocated word string or NULL if no word found
 */
char	*extract_word(const char *input, size_t *index,
			int is_delimiter_expected)
{
	size_t	start;
	char	*word;
	int		found_equals;
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	start = *index;
	found_equals = 0;
	quote_char = 0;
	while (input[*index] && (!ft_isspace(input[*index]) || in_quotes))
	{
		if (handle_quotes_and_special(input, index, &in_quotes, &quote_char))
			found_equals = 1;
		if (!is_delimiter_expected && !found_equals && *index > start
			&& is_special_char(input[*index]) && input[*index] != '$'
			&& !in_quotes)
			break ;
		(*index)++;
	}
	if (*index == start)
		return (NULL);
	word = ft_strndup(input + start, *index - start);
	return (word);
}

/*
 * Extracts operator tokens (pipes and redirections) from input
 * Handles single and double character operators (>, >>, <, <<, |)
 * Returns appropriate token or NULL if no operator found
 */
t_token	*extract_operator(const char *input, size_t *index)
{
	if (input[*index] == '|')
	{
		(*index)++;
		return (create_token(TOKEN_PIPE, "|"));
	}
	if (input[*index] == '>' || input[*index] == '<')
	{
		if (input[*index + 1] == '>')
			return ((*index) += 2, create_token(TOKEN_APPEND_OUT, ">>"));
		else if (input[*index] == '>')
			return ((*index)++, create_token(TOKEN_REDIR_OUT, ">"));
		else if (input[*index + 1] == '<')
			return ((*index) += 2, create_token(TOKEN_HEREDOC, "<<"));
		else
			return ((*index)++, create_token(TOKEN_REDIR_IN, "<"));
	}
	return (NULL);
}

/*
 * Validates that redirection operator has valid filename following it
 * Skips whitespace and checks for filename (not another operator or EOF)
 * Returns 0 on valid syntax, -1 on error with error message
 */
int	check_next_token(t_lexer *lexer, size_t next_pos)
{
	while (lexer->input[next_pos] && ft_isspace(lexer->input[next_pos]))
		next_pos++;
	if (!lexer->input[next_pos] || lexer->input[next_pos] == '>'
		|| lexer->input[next_pos] == '<' || lexer->input[next_pos] == '|')
		return (error_message("syntax error: redirection requires a filename"),
			-1);
	return (0);
}

/*
 * Tato funkce je "chamtivá". Čte vstupní řetězec a zastaví se až na konci slova.
 * Konec slova je definován jako mezera nebo operátor, který není uvnitř uvozovek.
 * Správně handleuje neuzavřené uvozovky.
*/
char *extract_greedy_word(const char *input, size_t *index)
{
    size_t start;
    int in_quotes;
    char quote_type;

    start = *index;
    in_quotes = 0;
    quote_type = 0;

    while (input[*index] != '\0')
    {
        char c = input[*index];

        // Zpracování uvozovek
        if (c == '\'' || c == '"')
        {
            if (in_quotes == 0) // Vcházíme do uvozovek
            {
                in_quotes = 1;
                quote_type = c;
            }
            else if (c == quote_type) // Vycházíme z uvozovek
            {
                in_quotes = 0;
            }
        }
        // Konec slova, pokud nejsme v uvozovkách
        else if (in_quotes == 0 && (ft_isspace(c) || c == '|' || c == '<' || c == '>'))
        {
            break;
        }
        (*index)++;
    }

    // KONTROLA CHYBY: Pokud jsme na konci vstupu a uvozovka je stále otevřená...
    if (in_quotes != 0)
    {
        error_message("syntax error: missing closing quote");
        return (NULL); // Toto je klíčové! Signalizujeme chybu.
    }

    // Vytvoříme a vrátíme podřetězec, který jsme našli.
    return (ft_strndup(input + start, *index - start));
}

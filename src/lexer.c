/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/31 08:51:06 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
 * Simple handler for special tokens (operators and pipes)
 * Processes redirections and pipes without complex compound logic
 */
static int	handle_special_tokens(t_lexer *lexer, int *is_first_word)
{
    if (lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
        return (process_redirections(lexer));
    else if (lexer->input[lexer->i] == '|')
        return (handle_pipe_token(lexer, is_first_word));
    else
        (lexer->i)++;
    return (0);
}

// Helper to append character to string using ft_strjoin
static char *append_char_to_string(char *str, char c)
{
    char char_str[2];
    char *result;
    
    char_str[0] = c;
    char_str[1] = '\0';
    
    if (!str)
        result = ft_strdup(char_str);
    else
    {
        result = ft_strjoin(str, char_str);
        free(str);
    }
    return (result);
}

// Helper to append string to string
static char *append_string_to_string(char *str, char *to_append)
{
    char *result;
    
    if (!str)
        result = ft_strdup(to_append);
    else
    {
        result = ft_strjoin(str, to_append);
        free(str);
    }
    return (result);
}

static char	*process_variable_expansion(t_lexer *lexer, char *token,
		t_shell *shell)
{
	char	var_name[256];
	char	*var_value;
	char	*exit_status_str;
	int		j;

	printf("DEBUG: expansion start at pos %zu, char='%c'\n", lexer->i, lexer->input[lexer->i]);
    
	j = 0;
	lexer->i++; // Přeskočení '$'
	if (lexer->input[lexer->i] == '?')
	{
		lexer->i++;
		exit_status_str = ft_itoa(shell->last_exit_code);
		token = append_string_to_string(token, exit_status_str);
		free(exit_status_str);
		return (token);
	}
	if (!ft_isalpha(lexer->input[lexer->i]) && lexer->input[lexer->i] != '_')
	{
		token = append_char_to_string(token, '$');
		return (token);
	}
	while (lexer->input[lexer->i]
		&& (ft_isalnum(lexer->input[lexer->i]) || lexer->input[lexer->i] == '_'))
	{
		if (j < 255)
			var_name[j++] = lexer->input[lexer->i];
		lexer->i++;
	}
	var_name[j] = '\0';
	var_value = env_get(shell->env, var_name);
	if (var_value)
		token = append_string_to_string(token, var_value);
	printf("DEBUG: expansion end at pos %zu, char='%c'\n", lexer->i, lexer->input[lexer->i]);
	return (token);
}

static char	*process_single_quotes(t_lexer *lexer, char *token)
{
	const char	*start;

	lexer->i++; // Přeskoč úvodní '
	start = &lexer->input[lexer->i];
	while (lexer->input[lexer->i] && lexer->input[lexer->i] != '\'')
		lexer->i++;
	if (lexer->input[lexer->i] == '\0')
	{
    	error_message("syntax error: unterminated quoted string\n");
    	free(token);
    	return (ft_strdup(""));  // ✅ Vrať prázdný string místo NULL
	}
	char *content = ft_strndup(start, &lexer->input[lexer->i] - start);
	token = append_string_to_string(token, content);
	free(content);
	lexer->i++; // Přeskoč závěrečnou '
	return (token);
}

static char *process_double_quotes(t_lexer *lexer, char *token, t_shell *shell)
{
    // size_t start_pos = lexer->i + 1; // Pozice po "
    
    lexer->i++; // Skip opening "
    
    // ✅ Stejná logika jako single quotes
    while (lexer->input[lexer->i] && lexer->input[lexer->i] != '"')
    {
        if (lexer->input[lexer->i] == '$')
        {
            token = process_variable_expansion(lexer, token, shell);
            if (!token)
                return (NULL);
        }
        else
        {
            token = append_char_to_string(token, lexer->input[lexer->i]);
            lexer->i++;
        }
    }
    
    // ✅ Kontrola PO while loopu - stejně jako single quotes
    if (lexer->input[lexer->i] == '\0')
    {
        error_message("syntax error: unterminated quoted string");
        free(token);
        return (ft_strdup(""));
    }
    
    lexer->i++; // Skip closing "
    return (token);
}

static char	*extract_complete_word(t_lexer *lexer, t_shell *shell)
{
	char	*token;

	token = NULL;
	while (lexer->input[lexer->i] && !ft_isspace(lexer->input[lexer->i])
		&& lexer->input[lexer->i] != '|' && lexer->input[lexer->i] != '<'
		&& lexer->input[lexer->i] != '>')
	{
		if (lexer->input[lexer->i] == '\'')
			token = process_single_quotes(lexer, token);
		else if (lexer->input[lexer->i] == '"')
			token = process_double_quotes(lexer, token, shell);
		else if (lexer->input[lexer->i] == '$')
			token = process_variable_expansion(lexer, token, shell);
		else
		{
			token = append_char_to_string(token, lexer->input[lexer->i]);
			lexer->i++;
		}
		if (!token)
			return (NULL);
	}
	return (token);
}

void	add_token_from_input(t_lexer *lexer, int *is_first_word, t_shell *shell)
{
	char	*word;
	int		type;

	word = extract_complete_word(lexer, shell);
	if (!word)
		return ;
	if (ft_strlen(word) == 0)
	{
    	free(word);
    	return ;  // Ignoruj prázdné tokeny
	}
	if (lexer->is_delimiter_expected)
		type = TOKEN_DELIMITER;
	else if (lexer->is_file_expected)
		type = TOKEN_FILE;
	else if (*is_first_word)
		type = TOKEN_CMD;
	else
		type = TOKEN_ARG;
	add_token(&lexer->tokens, create_token(type, word));
	if (*is_first_word)
		*is_first_word = 0;
	lexer->is_delimiter_expected = 0;
	lexer->is_file_expected = 0;
	free(word);
}

static int	token_lstsize(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

// V souboru lexer.c nahraďte funkci lexer touto verzí

t_token *lexer(const char *input, t_shell *shell)
{
    t_lexer lexer;
    int     is_first_word;

    is_first_word = 1;
    init_lexer(&lexer, input);
    while (lexer.input[lexer.i])
    {
        skip_whitespace(lexer.input, &(lexer.i));
        if (lexer.input[lexer.i] == '\0')
            break;
        int token_count_before = token_lstsize(lexer.tokens);
        if (is_special_char(lexer.input[lexer.i]))
        {
            if (handle_special_tokens(&lexer, &is_first_word) == -1)
            {
                free_tokens(lexer.tokens);
                return (NULL);
            }
        }
        else
        {
            add_token_from_input(&lexer, &is_first_word, shell);
            if (token_count_before == token_lstsize(lexer.tokens) && lexer.input[lexer.i] != '\0')
            {
                 free_tokens(lexer.tokens);
                 return (NULL);
            }
        }
    }
    return (lexer.tokens);
}

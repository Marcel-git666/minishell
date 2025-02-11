/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/11 16:49:54 by mmravec          ###   ########.fr       */
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
	if (lexer->is_file_expected) // ✅ Convert to FILE if needed
	{
		add_token(&lexer->tokens, create_token(TOKEN_FILE, word));
		lexer->is_file_expected = 0; // Reset flag
	}
	else if (lexer->is_delimiter_expected)
	{
		add_token(&lexer->tokens, create_token(TOKEN_DELIMITER, word));
		lexer->is_delimiter_expected = 0; // Reset flag
		*is_first_word = 1;
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

int	process_redirections(t_lexer *lexer, int *is_first_word)
{
	if ((lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
		&& lexer->input[lexer->i + 1] == lexer->input[lexer->i] // << or >>
		&& lexer->input[lexer->i + 2] != '\0' // Check if more characters exist
		&& !ft_isspace(lexer->input[lexer->i + 2]) // Must be followed by space or valid word
		&& !ft_isalnum(lexer->input[lexer->i + 2])) // Must not be another special character
	{
		error_message("syntax error near unexpected token");
		return (-1);
	}
	if (lexer->input[lexer->i] == '<' && lexer->input[lexer->i + 1] == '<')
	{
		lexer->i += 2;
		add_token(&(lexer->tokens), create_token(TOKEN_HEREDOC, "<<"));
		lexer->is_delimiter_expected = 1;
		*is_first_word = 1;
	}
	if (lexer->input[lexer->i] == '>' && lexer->input[lexer->i + 1] == '>')
	{
		lexer->i += 2;
		add_token(&(lexer->tokens), create_token(TOKEN_APPEND_OUT, ">>"));
		lexer->is_file_expected = 1;
	}
	else if (lexer->input[lexer->i] == '>')
	{
		lexer->i += 1;
		add_token(&(lexer->tokens), create_token(TOKEN_REDIR_OUT, ">"));
		lexer->is_file_expected = 1;
	}
	else if (lexer->input[lexer->i] == '<')
	{
		lexer->i += 1;
		add_token(&(lexer->tokens), create_token(TOKEN_REDIR_IN, "<"));
		lexer->is_file_expected = 1;
	}
	else
		return (-1);
	if (!lexer->is_delimiter_expected)
		*is_first_word = 0;
	return (0);
}

static int	handle_special_tokens(t_lexer *lexer, int *is_first_word)
{
	t_token	*op;
	char	*env;
	char	*quoted;

	if (lexer->input[lexer->i] == '\'' || lexer->input[lexer->i] == '"')
	{
		quoted = extract_quoted_string(lexer->input, &(lexer->i));
		if (!quoted)
		{
			error_message("syntax error: missing closing quote");
			return (-1);
		}
		add_token(&(lexer->tokens), create_token(TOKEN_STRING, quoted));
		free(quoted);
	}
	else if (lexer->input[lexer->i] == '>' || lexer->input[lexer->i] == '<')
	{
		if (process_redirections(lexer, is_first_word) == -1)
			return (-1);
	}
	else if (lexer->input[lexer->i] == '|' && lexer->input[lexer->i + 1] == '|')
	{
		error_message("syntax error near unexpected token `||`");
		return (-1);
	}
	else if (lexer->input[lexer->i] == '|')
	{
		if (lexer->i == 0 || lexer->input[lexer->i + 1] == '\0')
		{
			error_message("syntax error: `|` cannot start or end a command");
			return (-1);
		}
		if (!ft_isalpha(lexer->input[lexer->i + 1])
			&& lexer->input[lexer->i + 1] != '_')
		{
			error_message("syntax error: `|` must be followed by a valid command");
			return (-1);
		}
		op = extract_operator(lexer->input, &(lexer->i));
		if (op)
		{
			add_token(&(lexer->tokens), op);
			*is_first_word = 1;
		}
	}
	else if (lexer->input[lexer->i] == '$')
	{
		env = extract_env_var(lexer->input, &(lexer->i));
		add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env));
		free(env);
	}
	else
		(lexer->i)++;
	return (0);
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
		if (ft_isalnum(lexer.input[lexer.i]) || lexer.input[lexer.i] == '-'
			|| lexer.input[lexer.i] == '/'
			|| lexer.input[lexer.i] == '.' || lexer.input[lexer.i] == '~')
			add_token_from_input(&lexer, &is_first_word);
		else if (handle_special_tokens(&lexer, &is_first_word) == -1)
			return (NULL);
	}
	return ((lexer.tokens));
}

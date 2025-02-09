/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/09 21:24:07 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else if (*is_first_word)
		add_token(&(lexer->tokens), create_token(TOKEN_CMD, word));
	else
		add_token(&(lexer->tokens), create_token(TOKEN_ARG, word));
	free(word);
	*is_first_word = 0;
}

static void	handle_special_tokens(t_lexer *lexer, int *is_first_word)
{
	t_token	*op;
	char	*env;
	char	*quoted;

	if (lexer->input[lexer->i] == '\'' || lexer->input[lexer->i] == '"')
	{
		quoted = extract_quoted_string(lexer->input, &(lexer->i));
		add_token(&(lexer->tokens), create_token(TOKEN_STRING, quoted));
		free(quoted);
	}
	else if (lexer->input[lexer->i] == '|' || lexer->input[lexer->i] == '>'
		|| lexer->input[lexer->i] == '<')
	{
		op = extract_operator(lexer->input, &(lexer->i));
		if (op)
		{
			add_token(&(lexer->tokens), op);
			if (op->type == TOKEN_REDIR_OUT || op->type == TOKEN_REDIR_IN)
				lexer->is_file_expected = 1;
		}
		*is_first_word = 1; // Reset for next command
	}
	else if (lexer->input[lexer->i] == '$')
	{
		env = extract_env_var(lexer->input, &(lexer->i));
		add_token(&(lexer->tokens), create_token(TOKEN_ENV_VAR, env));
		free(env);
	}
	else
		(lexer->i)++;
}

t_token	*lexer(const char *input)
{
	t_lexer	lexer;
	int		is_first_word;

	lexer.i = 0;
	lexer.tokens = NULL;
	lexer.input = input;
	lexer.is_file_expected = 0;
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
		else
			handle_special_tokens(&lexer, &is_first_word);
	}
	return ((lexer.tokens));
}

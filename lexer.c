/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/07 19:20:38 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

void	add_token_from_input(t_token **tokens, const char *input,
		size_t *i, int *is_first_word)
{
	char	*word;

	word = extract_word(input, i);
	if (*is_first_word)
		add_token(tokens, create_token(TOKEN_CMD, word));
	else
		add_token(tokens, create_token(TOKEN_ARG, word));
	free(word);
	*is_first_word = 0;
}

void	handle_special_tokens(t_token **tokens, const char *input,
		size_t *i, int *is_first_word)
{
	t_token	*op;
	char	*env;
	char	*quoted;

	if (input[*i] == '\'' || input[*i] == '"')
	{
		quoted = extract_quoted_string(input, i);
		add_token(tokens, create_token(TOKEN_STRING, quoted));
		free(quoted);
	}
	else if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<')
	{
		op = extract_operator(input, i);
		if (op)
			add_token(tokens, op);
		*is_first_word = 1; // Reset for next command
	}
	else if (input[*i] == '$')
	{
		env = extract_env_var(input, i);
		add_token(tokens, create_token(TOKEN_ENV_VAR, env));
		free(env);
	}
	else
		(*i)++;
}

t_token	*lexer(const char *input)
{
	size_t	i;
	t_token	*tokens;
	int		is_first_word;

	i = 0;
	tokens = NULL;
	is_first_word = 1;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (input[i] == '\0')
			break ;
		if (ft_isalnum(input[i]))
			add_token_from_input(&tokens, input, &i, &is_first_word);
		else
			handle_special_tokens(&tokens, input, &i, &is_first_word);
	}
	return (tokens);
}

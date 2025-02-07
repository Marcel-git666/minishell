/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:45:44 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/07 17:38:32 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(const char *input)
{
	size_t	i;
	t_token	*tokens;
	char	*word;
	char	*quoted;
	t_token	*op;
	char	*env;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (isalpha(input[i]))
		{
			word = extract_word(input, &i);
			add_token(&tokens, create_token(TOKEN_CMD, word));
			free(word);
		}
		else if (input[i] == '\'' || input[i] == '"')
		{
			quoted = extract_quoted_string(input, &i);
			add_token(&tokens, create_token(TOKEN_STRING, quoted));
			free(quoted);
		}
		else if (input[i] == '|' || input[i] == '>' || input[i] == '<')
		{
			op = extract_operator(input, &i);
			if (op)
				add_token(&tokens, op);
		}
		else if (input[i] == '$')
		{
			env = extract_env_var(input, &i);
			add_token(&tokens, create_token(TOKEN_ENV_VAR, env));
			free(env);
		}
		else
		{
			i++;
		}
	}
	return (tokens);
}

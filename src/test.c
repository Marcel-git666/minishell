/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:49:01 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/19 18:03:20 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	assert_token(t_token *token, t_token_type expected_type,
			char *expected_value)
{
	if (!token)
	{
		printf("%sFAIL%s: Expected token but got NULL\n", RED, RESET);
		return ;
	}
	if (token->type != expected_type || ft_strncmp(token->value, expected_value,
			ft_strlen(expected_value) + 1) != 0)
	{
		printf("%sFAIL%s: Expected [%d: \"%s\"] but got [%d: \"%s\"]\n",
			RED, RESET, expected_type, expected_value, token->type,
			token->value);
		return ;
	}
	printf("%sPASS%s: Token [%d: \"%s\"]\n", GREEN, RESET, token->type,
		token->value);
}

void	test_basic_cmd(void)
{
	t_token	*tokens;

	printf("\nTesting basic commands:\n");
	tokens = lexer("ls -la");
	assert_token(tokens, TOKEN_CMD, "ls");
	assert_token(tokens->next, TOKEN_ARG, "-la");
	free_tokens(tokens);
}

void	test_complex_cmd(void)
{
	t_token	*tokens;

	printf("\nTesting: ls -la  |cat -e > output.txt\n");
	tokens = lexer("ls -la  |cat -e > output.txt");
	assert_token(tokens, TOKEN_CMD, "ls");
	assert_token(tokens->next, TOKEN_ARG, "-la");
	tokens = tokens->next;
	assert_token(tokens->next, TOKEN_PIPE, "|");
	tokens = tokens->next;
	assert_token(tokens->next, TOKEN_CMD, "cat");
	tokens = tokens->next;
	assert_token(tokens->next, TOKEN_ARG, "-e");
	tokens = tokens->next;
	assert_token(tokens->next, TOKEN_REDIR_OUT, ">");
	tokens = tokens->next;
	assert_token(tokens->next, TOKEN_FILE, "output.txt");
	free_tokens(tokens);
}

void	test_lexer(void)
{
	t_token	*tokens;

	test_basic_cmd();
	test_complex_cmd();
	printf("\nTesting variable assignments:\n");
	tokens = lexer("VAR=value");
	assert_token(tokens, TOKEN_ASSIGNMENT, "VAR=value");
	free_tokens(tokens);

	tokens = lexer("VAR = value");
	if (!tokens)
		printf("%sPASS%s: Invalid assignment correctly returns NULL\n", GREEN, RESET);
	else
	{
		printf("%sFAIL%s: Invalid assignment should return NULL\n", RED, RESET);
		free_tokens(tokens);
	}
}

void	run_test_suite(void)
{
	printf("\n=== Running Test Suite ===\n");
	test_lexer();
	printf("\n=== Test Suite Complete ===\n");
}

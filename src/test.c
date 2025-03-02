/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:49:01 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/20 22:10:12 by mmravec          ###   ########.fr       */
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

void	assert_token_sequence(t_token *tokens, t_expected_token *expected,
		int count)
{
	int		i;
	t_token	*current;

	current = tokens;
	i = 0;
	printf("\nChecking token sequence:\n");
	while (i < count && current)
	{
		assert_token(current, expected[i].type, expected[i].value);
		current = current->next;
		i++;
	}
	if (i < count)
		printf("%sFAIL%s: Expected %d tokens but got only %d\n",
			RED, RESET, count, i);
	if (current)
		printf("%sFAIL%s: More tokens present than expected\n", RED, RESET);
}

void	test_basic_cmd(void)
{
	t_token	*tokens;
	t_expected_token expected[] = {
		{TOKEN_CMD, "ls"},
		{TOKEN_ARG, "-la"}
	};

	printf("\nTesting basic commands:\n");
	tokens = lexer("ls -la");
	assert_token_sequence(tokens, expected, 2);
	free_tokens(tokens);
}

void	test_complex_cmd(void)
{
	t_token	*tokens;
	t_expected_token expected[] = {
		{TOKEN_CMD, "ls"},
		{TOKEN_ARG, "-la"},
		{TOKEN_PIPE, "|"},
		{TOKEN_CMD, "cat"},
		{TOKEN_ARG, "-e"},
		{TOKEN_REDIR_OUT, ">"},
		{TOKEN_FILE, "output.txt"}
	};

	printf("\nTesting: ls -la  |cat -e > output.txt\n");
	tokens = lexer("ls -la  |cat -e > output.txt");
	assert_token_sequence(tokens, expected, 7);
	free_tokens(tokens);
}


void	run_test_suite(void)
{
	printf("\n=== Running Test Suite ===\n");
	test_basic_cmd();
	test_complex_cmd();
	test_pipe_sequences();
	test_env_variables();
	test_heredoc_variations();
	test_mixed_quotes();
	test_mixed_redirections();
	printf("\n=== Test Suite Complete ===\n");
}

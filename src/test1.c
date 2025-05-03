/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:06:22 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/04 02:12:16 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "test.h"

void	test_pipe_sequences(void)
{
	t_token	*tokens;
	t_expected_token	expected[] = {
		{TOKEN_CMD, "ls"},
		{TOKEN_ARG, "-la"},
		{TOKEN_PIPE, "|"},
		{TOKEN_CMD, "grep"},
		{TOKEN_STRING, "*.c"},
		{TOKEN_PIPE, "|"},
		{TOKEN_CMD, "wc"},
		{TOKEN_ARG, "-l"}
	};

	printf("\nTesting pipe sequence: ls -la | grep \"*.c\" | wc -l\n");
	tokens = lexer("ls -la | grep \"*.c\" | wc -l");
	assert_token_sequence(tokens, expected, 8);
	free_tokens(tokens);
}

void	test_mixed_redirections(void)
{
	t_token	*tokens;
	t_expected_token	expected[] = {
		{TOKEN_CMD, "cat"},
		{TOKEN_REDIR_IN, "<"},
		{TOKEN_FILE, "input.txt"},
		{TOKEN_PIPE, "|"},
		{TOKEN_CMD, "sort"},
		{TOKEN_REDIR_OUT, ">"},
		{TOKEN_FILE, "output.txt"}
	};

	printf("\nTesting mixed redirections: cat < input.txt | sort > output.txt\n");
	tokens = lexer("cat < input.txt | sort > output.txt");
	assert_token_sequence(tokens, expected, 7);
	free_tokens(tokens);
}

void	test_env_variables(void)
{
	t_token	*tokens;
	t_expected_token	expected[] = {
		{TOKEN_CMD, "echo"},
		{TOKEN_STRING, "Hello "},
		{TOKEN_ENV_VAR, "USER"},
		{TOKEN_STRING, "! Your home is "},
		{TOKEN_ENV_VAR, "HOME"}
	};

	printf("\nTesting env variables in quotes:\n");
	tokens = lexer("echo \"Hello $USER! Your home is $HOME\"");
	assert_token_sequence(tokens, expected, 5);
	free_tokens(tokens);
}

void	test_mixed_quotes(void)
{
	t_token	*tokens;
	t_expected_token	expected[] = {
		{TOKEN_CMD, "echo"},
		{TOKEN_STRING, "$USER"},  // Should not expand in single quotes
		{TOKEN_ENV_VAR, "HOME"}   // Should expand in double quotes
	};

	printf("\nTesting mixed quotes handling:\n");
	tokens = lexer("echo '$USER' \"$HOME\"");
	assert_token_sequence(tokens, expected, 3);
	free_tokens(tokens);
}

void	test_heredoc_variations(void)
{
	t_token	*tokens;
	t_expected_token	expected[] = {
		{TOKEN_CMD, "cat"},
		{TOKEN_HEREDOC, "<<"},
		{TOKEN_DELIMITER, "EOF"}
	};

	printf("\nTesting heredoc basic:\n");
	tokens = lexer("cat << EOF");
	assert_token_sequence(tokens, expected, 3);
	free_tokens(tokens);

	printf("\nTesting heredoc with special chars in delimiter:\n");
	expected[2].value = "$$$$";
	tokens = lexer("cat << $$$$");
	assert_token_sequence(tokens, expected, 3);
	free_tokens(tokens);
}

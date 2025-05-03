/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:48:09 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/04 02:06:03 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "token.h"

typedef struct s_expected_token
{
	t_token_type	type;
	char			*value;
}	t_expected_token;

void	assert_token_sequence(t_token *tokens, t_expected_token *expected,
			int count);
void	assert_token(t_token *token, t_token_type expected_type,
			char *expected_value);
void	run_test_suite(void);

// test1.c
void	test_pipe_sequences(void);
void	test_mixed_redirections(void);
void	test_env_variables(void);
void	test_mixed_quotes(void);
void	test_heredoc_variations(void);

// Color codes for test output
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define RESET "\033[0m"

#endif

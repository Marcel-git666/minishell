/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:48:09 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/19 17:48:42 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "minishell.h"

void	test_lexer(void);
void	assert_token(t_token *token, t_token_type expected_type,
			char *expected_value);
void	run_test_suite(void);

// Color codes for test output
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define RESET "\033[0m"

#endif

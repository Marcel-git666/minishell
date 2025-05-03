/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/04 02:07:02 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "test.h"

void	print_tokens(t_token *tokens)
{
	const char	*token_labels[] =
	{
		"TOKEN_CMD", "TOKEN_ARG", "TOKEN_PIPE", "TOKEN_REDIR_IN",
		"TOKEN_REDIR_OUT", "TOKEN_APPEND_OUT", "TOKEN_HEREDOC",
		"TOKEN_STRING", "TOKEN_ENV_VAR", "TOKEN_EXIT_CODE",
		"TOKEN_FILE", "TOKEN_DELIMITER", "TOKEN_ASSIGNMENT", "TOKEN_EOF"
	};

	while (tokens)
	{
		printf("[%s: \"%s\"]\n", token_labels[tokens->type], tokens->value);
		tokens = tokens->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	if (argc > 1 && ft_strncmp(argv[1], "--test", 7) == 0)
	{
		run_test_suite();
		return (0);
	}
	initialize_shell(envp);
	run_shell_loop();
	save_history();
	printf("Exiting minishell...\n");
	return (0);
}

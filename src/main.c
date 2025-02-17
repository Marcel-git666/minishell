/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/10 18:24:32 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	const char *token_labels[] = {
			"TOKEN_CMD", "TOKEN_ARG", "TOKEN_PIPE", "TOKEN_REDIR_IN",
			"TOKEN_REDIR_OUT", "TOKEN_APPEND_OUT", "TOKEN_HEREDOC",
			"TOKEN_STRING", "TOKEN_ENV_VAR", "TOKEN_EXIT_CODE",
			"TOKEN_FILE", "TOKEN_DELIMITER", "TOKEN_EOF"
		};

	while (tokens)
	{
		printf("[%s: \"%s\"]\n", token_labels[tokens->type], tokens->value);
		tokens = tokens->next;
	}
}

int	main(void)
{
	initialize_shell();
	run_shell_loop();
	save_history();
	printf("Exiting minishell...\n");
	return (0);
}

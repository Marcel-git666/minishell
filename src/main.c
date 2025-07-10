/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/22 13:07:15 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "test.h"

void	print_tokens(t_token *tokens)
{
	const char *token_labels[] =
    {
        "TOKEN_CMD",           // 0
        "TOKEN_ARG",           // 1
        "TOKEN_PIPE",          // 2  
        "TOKEN_REDIR_IN",      // 3
        "TOKEN_REDIR_OUT",     // 4
        "TOKEN_APPEND_OUT",    // 5
        "TOKEN_HEREDOC",       // 6
        "TOKEN_DOUBLE_QUOTED", // 7
        "TOKEN_SINGLE_QUOTED", // 8
        "TOKEN_ENV_VAR",       // 9
        "TOKEN_EXIT_CODE",     // 10
        "TOKEN_FILE",          // 11
        "TOKEN_DELIMITER",     // 12
        "TOKEN_ASSIGNMENT",    // 13
        "TOKEN_EOF"            // 14
    };

	while (tokens)
	{
		printf("[%s: \"%s\"]\n", token_labels[tokens->type], tokens->value);
		tokens = tokens->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	
	(void)argc;
	(void)argv;
	shell = initialize_shell(envp);
	if (!shell)
		return (1);
	run_shell_loop(shell, envp);
	save_history();
	env_free(shell->env);
	free(shell);
	printf("Exiting minishell...\n");
	return (0);
}

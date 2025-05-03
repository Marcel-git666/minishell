/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:59:07 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/03 18:55:56 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_shell(char **envp)
{
	printf("Welcome to mini shell!\n");
	env_init(envp);
	load_history();
}

void	handle_input(char *input)
{
	static char			*last_executed = NULL;
	struct _hist_entry	**history_array;

	history_array = history_list();
	if (*input)
	{
		if (history_length == 0 ||
			(history_array && history_array[history_length - 1] &&
				ft_strncmp(input, history_array[history_length - 1]->line,
					ft_strlen(input) + 1) != 0 && (!last_executed
					|| ft_strncmp(input, last_executed,
						ft_strlen(input) + 1) != 0)))
		{
			add_history(input);
			save_history();
		}
		free(last_executed);
		last_executed = ft_strdup(input);
	}
}

void	run_shell_loop(void)
{
	char		*input;
	t_token		*tokens;
	char		*prompt;
	t_ast_node	*ast;

	setup_signals();
	while (1)
	{
		if (g_signal_received)
			prompt = "";
		else
			prompt = "$ ";
		input = readline(prompt);
		if (!input)
			break ;
		g_signal_received = 0;
		if (*input)
		{
			handle_input(input);
			tokens = lexer(input);
			if (tokens)
			{
				print_tokens(tokens);
				ast = parse_tokens(tokens);
				if (ast)
				{
					// For now, just print that parsing succeeded
					printf("Successfully created AST\n");
					print_ast(ast, 0);
					execute_command(ast);
					free_ast(ast);
				}
				free_tokens(tokens);
			}
		}
		free(input);
	}
}

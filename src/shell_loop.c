/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:59:07 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 14:07:53 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*initialize_shell(char **envp)
{
	t_shell *shell;
    
    shell = malloc(sizeof(t_shell));
    if (!shell)
	{
        error_message("Failed to allocate shell state");
		return (NULL);
	}
	printf("Welcome to mini shell!\n");
	shell->env = env_init(envp);
	if (!shell->env)
	{
		free(shell);
		error_message("Failed to initialize environment");
		return (NULL);
	}
	shell->last_exit_code = 0;
	env_print(shell->env);
	load_history();
	return (shell);
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

void	run_shell_loop(t_shell *shell)
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
					// redirection(ast, shell);
					execute_command(ast, shell);
					free_ast(ast);
				}
				free_tokens(tokens);
			}
		}
		free(input);
	}
}

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
	t_shell	*shell;

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
	env_print(shell);
	load_history();
	return (shell);
}

static void	process_tokens_and_execute(t_token *tokens, t_shell *shell,
				char **envp)
{
	t_ast_node	*ast;

	if (!tokens)
		return ;
	print_tokens(tokens);
	ast = parse_tokens(tokens);
	if (ast)
	{
		printf("Successfully created AST\n");
		print_ast(ast, 0);
		execute_command(ast, shell, envp);
		free_ast(ast);
	}
	free_tokens(tokens);
}

static void	process_input_line(char *input, t_shell *shell, char **envp)
{
	t_token	*tokens;

	if (!*input)
		return ;
	handle_input(input);
	tokens = lexer(input);
	process_tokens_and_execute(tokens, shell, envp);
}

static char	*get_prompt(void)
{
	if (g_signal_received)
		return ("");
	else
		return ("$ ");
}

void	run_shell_loop(t_shell *shell, char **envp)
{
	char	*input;
	char	*prompt;

	setup_signals();
	while (1)
	{
		prompt = get_prompt();
		input = readline(prompt);
		if (!input)
			break ;
		g_signal_received = 0;
		process_input_line(input, shell, envp);
		free(input);
	}
}

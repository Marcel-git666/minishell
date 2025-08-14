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

/*
 * Initializes shell state with environment and history
 * Creates shell structure, loads environment variables, and sets up history
 * Returns shell pointer on success, NULL on failure
 */
t_shell	*initialize_shell(char **envp)
{
	t_shell	*shell;

	if (!envp)
		return (NULL);
	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		error_message("Failed to allocate shell state");
		return (NULL);
	}
	shell->last_executed = NULL;
	shell->ast = NULL;
	shell->last_exit_code = 0;
	printf("Welcome to mini shell!\n");
	shell->env = env_init(envp);
	if (!shell->env)
	{
		free(shell);
		error_message("Failed to initialize environment");
		return (NULL);
	}
	load_history();
	return (shell);
}

/*
 * Processes token list into AST and executes commands
 * Handles token validation, AST creation, execution, and cleanup
 */
static void	process_tokens_and_execute(t_token *tokens, t_shell *shell,
				char **envp)
{
	t_ast_node	*ast;
	t_parser	parser;

	if (!tokens || !shell)
	{
		if (tokens)
			free_tokens(tokens);
		if (shell)
			shell->last_exit_code = 2;
		return ;
	}
	init_parser(tokens, &parser);
	ast = parse_expression(&parser);
	if (parser.error || !ast)
	{
		if (parser.error && parser.error_msg)
		{
			error_message(parser.error_msg);
			free(parser.error_msg);
			parser.error_msg = NULL;
		}
		shell->last_exit_code = 2;
		free_tokens(tokens);
		if (ast)
			free_ast(ast);
		return ;
	}
	free_tokens(tokens);
	execute_command(ast, shell, envp);
	free_ast(ast);
	ast = NULL;
	if (shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
}

/*
 * Processes a single input line from user
 * Handles empty input, history, lexing, and command execution
 */
static void	process_input_line(char *input, t_shell *shell, char **envp)
{
	t_token	*tokens;

	if (!input || !shell || !*input)
		return ;
	handle_input(input, shell);
	tokens = lexer(input);
	process_tokens_and_execute(tokens, shell, envp);
}

/*
 * Returns appropriate prompt based on signal state
 * Empty prompt after signal, normal prompt otherwise
 */
static char	*get_prompt(void)
{
	return ("$ ");
}

/*
 * Main shell loop - reads input, processes commands, handles signals
 * Continues until EOF (Ctrl+D) or exit command
 */
void	run_shell_loop(t_shell *shell, char **envp)
{
	char	*input;
	char	*prompt;

	if (!shell || !envp)
		return ;
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
		input = NULL;
	}
}

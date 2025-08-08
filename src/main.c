/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/08 11:42:32 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Main entry point for minishell
 * Initializes shell state, runs interactive loop, and handles cleanup
 * Returns 0 on normal exit, 1 on initialization failure
 */
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
	rl_clear_history();
	env_free(shell->env);
	cleanup_resources(shell, NULL, NULL);
	return (0);
}

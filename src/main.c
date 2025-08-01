/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 18:14:19 by marcel           ###   ########.fr       */
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
	env_free(shell->env);
	free(shell);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:59:08 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 11:01:57 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <termios.h>

int	g_signal_received = 0;

/*
 * Handles SIGINT (Ctrl+C) in interactive mode
 * Sets global signal flag, prints newline, and resets readline
 */
static void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal_received = signum;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*
 * Sets up signal handlers for interactive shell
 * Handles SIGINT with custom handler, ignores SIGQUIT
 */
void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Signal handler for heredoc child process
 * Exits immediately on SIGINT to terminate heredoc input
 */
void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		exit(130);
	}
}

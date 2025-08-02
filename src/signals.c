/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:59:08 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/02 11:35:48 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <termios.h>

int	g_signal_received = 0;
extern int	g_signal_heredoc;
/*
 * Handles SIGINT (Ctrl+C) in interactive mode
 * Sets global signal flag, prints newline, and resets readline
 */
void	signal_handler(int signum)
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
		g_signal_heredoc = signum;
	}
	else if (signum == SIGQUIT)
	{
		error_message("Quit: 3");
		exit(131);
	}
}

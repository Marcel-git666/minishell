/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:59:08 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/17 19:59:40 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <termios.h>

static void	signal_handler(int signum)
{
	if (signum == SIGINT)  // Ctrl-C
    {
        g_signal_received = signum;
        write(1, "\n", 1);     // New line
        rl_on_new_line();      // Inform readline we moved to new line
        rl_replace_line("", 0); // Clear the current line
        rl_redisplay();        // Show new prompt
    }
}

void setup_signals(void)
{
    struct sigaction sa;

    // Initialize sigaction struct
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  // Restart interrupted system calls

    // Set up signal handlers
    sigaction(SIGINT, &sa, NULL);  // Ctrl-C
    signal(SIGQUIT, SIG_IGN);      // Ctrl-\: Ignore it
}

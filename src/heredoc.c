/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:46:06 by lformank          #+#    #+#             */
/*   Updated: 2025/08/13 17:56:02 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection.h"

extern int	g_signal_heredoc;

int	no_newline(char *newline, t_fds *fd)
{
	if (!newline)
	{
		if (g_signal_heredoc)
			return (0);
		write(fd->out_old, "bash: warning: here-document at line 1\
delimited by end-of-file (wanted `EOF')\n", 79);
		return (0);
	}
	return (1);
}

void	child_heredoc(char *delimiter, t_fds *fd, t_shell *shell)
{
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
	read_loop(delimiter, fd);
	close(fd->here_new);
	free(delimiter);
	cleanup_resources(shell, fd, shell->ast);
}

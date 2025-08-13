/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:53:23 by lformank          #+#    #+#             */
/*   Updated: 2025/06/04 14:38:34 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection.h"
#include <errno.h>
#include <termios.h>

/*
 * Reads heredoc input from user until delimiter is encountered
 * Creates temporary file and writes input lines to it
 * Handles EOF and delimiter matching for termination
 */
void	read_loop(char *delimiter, int temp_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal_received)
		{
			if (line)
				free(line);
			break ;
		}
		if (!line) // Reakce na Ctrl+D (EOF)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, temp_fd);
		free(line);
	}
}

/*
 * Logika rodičovského procesu po forku pro heredoc.
 * Čeká na child a zkontroluje jeho exit status.
 */
static int	heredoc_parent(pid_t pid, t_shell *shell)
{
	int	status;

	signal(SIGINT, SIG_IGN); // Ignorujeme signál, zatímco čekáme
	waitpid(pid, &status, 0);
	setup_signals(); // Obnovíme původní signál handlery
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		shell->last_exit_code = 130;
		g_signal_received = SIGINT;
		return (-1); // Přerušeno signálem
	}
	return (0); // Vše v pořádku
}

/*
 * Hlavní funkce pro heredoc. Vytvoří dočasný soubor a forkuje proces pro čtení vstupu.
 * Upraví uzel přesměrování tak, aby ukazoval na dočasný soubor.
 */
int	heredoc(t_shell *shell, t_redirection *redir, t_fds *fds)
{
	int				temp_fd;
	pid_t			pid;
	struct termios	original_termios;

	// 1. Uložíme si původní nastavení terminálu.
	tcgetattr(STDIN_FILENO, &original_termios);

	// 2. Nahradíme mkstemp vlastní logikou.
	fds->temp = new_tempfile(); // Použijeme vaši funkci pro generování názvu.
	if (!fds->temp)
		return (-1);
	temp_fd = open(fds->temp, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (temp_fd == -1)
	{
		perror("minishell");
		free(fds->temp);
		fds->temp = NULL;
		return (-1);
	}
	
	g_signal_received = 0;
	pid = fork();
	if (pid == -1)
		return (tcsetattr(STDIN_FILENO, TCSANOW, &original_termios), -1);
	if (pid == 0) // Dceřiný proces
	{
		signal(SIGINT, signal_handler_heredoc);
		read_loop(redir->file_or_delimiter, temp_fd);
		close(temp_fd);
		exit(g_signal_received ? 1 : 0);
	}

	// Rodičovský proces
	close(temp_fd);
	if (heredoc_parent(pid, shell) == -1)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
		return (-1);
	}
	
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
	free(redir->file_or_delimiter);
	redir->file_or_delimiter = ft_strdup(fds->temp);
	if (!redir->file_or_delimiter)
		return (-1);
	redir->type = REDIR_IN;
	return (0);
}

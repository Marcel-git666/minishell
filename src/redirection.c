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

int	g_signal_heredoc = 0;

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
		if (g_signal_heredoc) // Reakce na Ctrl+C
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
	int		temp_fd;
	pid_t	pid;

	fds->temp = ft_strdup("/tmp/minishell_heredoc_XXXXXX"); // Šablona pro dočasný soubor
	temp_fd = mkstemp(fds->temp); // Vytvoří unikátní dočasný soubor
	if (temp_fd == -1)
		return (perror("minishell"), -1);
	g_signal_heredoc = 0;
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0) // Dceřiný proces (child)
	{
		signal(SIGINT, signal_handler_heredoc);
		read_loop(redir->file_or_delimiter, temp_fd);
		close(temp_fd);
		exit(g_signal_heredoc ? 130 : 0);
	}
	close(temp_fd);
	if (heredoc_parent(pid, shell) == -1)
		return (-1);
	// Nahradíme delimiter názvem dočasného souboru a změníme typ na vstupní přesměrování
	free(redir->file_or_delimiter);
	redir->file_or_delimiter = ft_strdup(fds->temp);
	redir->type = REDIR_IN;
	return (0);
}


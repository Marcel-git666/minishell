/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:41:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/01 10:34:39 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Gets history file path in user's home directory
 * Returns $HOME/.history or falls back to .history if HOME not set
 * Caller must free returned string
 */
static char	*get_history_path(void)
{
	char    *home;
	char	*path_with_slash;
	char	*full_path;

	home = getenv("HOME");
	if (!home)
		return (ft_strdup(HISTORY_FILE));
	path_with_slash = ft_strjoin(home, "/");
	if (!path_with_slash)
		return (ft_strdup(HISTORY_FILE));
	full_path = ft_strjoin(path_with_slash, HISTORY_FILE);
	free(path_with_slash);
	return (full_path);
}

/*
 * Loads command history from .history file on shell startup
 * Reads file content, tokenizes by newlines, and adds to readline history
 * Silently fails if file doesn't exist or can't be read
 */
void	load_history(void)
{
	char	*history;
	char	*line;
	char	*history_path;

	history = NULL;
	history_path = get_history_path();
	if (open_file(history_path, &history, O_RDONLY) == -1)
	{
		free(history_path);
		return ;
	}
	if (history)
	{
		line = ft_strtok(history, "\n");
		while (line)
		{
			add_history(line);
			line = ft_strtok(NULL, "\n");
		}
		free(history);
	}
	free(history_path);
}

/*
 * Saves current readline history to .history file
 * Creates/truncates file and writes each history entry on separate line
 * Handles empty history gracefully
 */
void	save_history(void)
{
	HIST_ENTRY	**h_array;
	int			i;
	int			fd;
	char		*history_path;

	if (history_length <= 0)
		return ;
	h_array = history_list();
	if (!h_array)
		return ;
	history_path = get_history_path();
	fd = open_file(history_path, NULL, O_WRONLY | O_CREAT | O_TRUNC);
	free(history_path);
	if (fd == -1)
		return ;
	i = -1;
	while (++i < history_length)
	{
		if (h_array[i] && h_array[i]->line)
		{
			write(fd, h_array[i]->line, ft_strlen(h_array[i]->line));
			write(fd, "\n", 1);
		}
	}
	close(fd);
}

/*
 * Determines if input should be added to history
 * Prevents duplicate consecutive entries and empty commands
 * Returns 1 to add, 0 to skip
 */
static int	should_add_to_history(char *input, char *last_executed)
{
	HIST_ENTRY	**history_array;

	history_array = history_list();
	if (history_length == 0)
		return (1);
	if (history_array && history_array[history_length - 1]
		&& ft_strncmp(input, history_array[history_length - 1]->line,
			ft_strlen(input) + 1) != 0
		&& (!last_executed || ft_strncmp(input, last_executed,
				ft_strlen(input) + 1) != 0))
		return (1);
	return (0);
}

/*
 * Handles user input for history management
 * Adds non-duplicate commands to history and saves to file
 * Maintains static reference to last executed command
 */
void	handle_input(char *input)
{
	static char	*last_executed = NULL;

	if (*input)
	{
		if (should_add_to_history(input, last_executed))
		{
			add_history(input);
			save_history();
		}
		free(last_executed);
		last_executed = ft_strdup(input);
	}
}

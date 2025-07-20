/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:41:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 11:26:08 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Loads command history from .history file on shell startup
 * Reads file content, tokenizes by newlines, and adds to readline history
 * Silently fails if file doesn't exist or can't be read
 */
void	load_history(void)
{
	char	*history;
	char	*line;

	history = NULL;
	if (open_file(HISTORY_FILE, &history, O_RDONLY) == -1)
		return ;
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
}

/*
 * Saves current readline history to .history file
 * Creates/truncates file and writes each history entry on separate line
 * Handles empty history gracefully
 */
void	save_history(void)
{
	HIST_ENTRY	**history_array;
	int			i;
	int			fd;
	int			history_count;

	history_count = history_length;
	if (history_count <= 0)
		return ;
	history_array = history_list();
	if (!history_array)
		return ;
	fd = open_file(HISTORY_FILE, NULL, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
		return ;
	i = -1;
	while (++i < history_count)
	{
		if (history_array[i] && history_array[i]->line)
		{
			write(fd, history_array[i]->line,
				ft_strlen(history_array[i]->line));
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

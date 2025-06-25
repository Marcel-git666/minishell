/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:41:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/22 23:00:37 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

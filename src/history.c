/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:41:39 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/09 12:22:26 by mmravec          ###   ########.fr       */
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
	struct _hist_entry	**history_array;
	int					i;
	int					fd;
	int					history_count;

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

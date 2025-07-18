/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manipulation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 21:44:07 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/18 23:31:16 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_to_content(char **file_content, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(*file_content, buffer);
	if (!temp)
	{
		free(*file_content);
		write(2, "Error: Memory allocation failed.\n", 34);
		return (0);
	}
	free(*file_content);
	*file_content = temp;
	return (1);
}

static int	create_and_open_file(char *name, int oflag, mode_t mode)
{
	int	fd;

	fd = open(name, oflag, mode);
	if (fd == -1 && (oflag & O_CREAT)) // ✅ Create file only if O_CREAT is used
	{
		fd = open(name, oflag | O_CREAT, mode);
		if (fd == -1)
			return (write(2, "Error: Could not open history file.\n", 36), -1);
	}
	return (fd);
}

static int	read_file_content(int fd, char **file_content)
{
	int		ret;
	char	buffer[4096 + 1];

	*file_content = ft_strdup("");
	if (!*file_content)
		return (write(2, "Error: Memory allocation failed.\n", 34), 0);
	ret = read(fd, buffer, 4096);
	while (ret > 0)
	{
		buffer[ret] = '\0';
		if (!append_to_content(file_content, buffer))
			return (-1);
		ret = read(fd, buffer, 4096);
	}
	close(fd);
	if (ret == -1)
		return (-1);
	return (1);
}

int	open_file(char *name, char **file_content, int oflag)
{
	int	fd;

	fd = create_and_open_file(name, oflag, 0644);
	if (fd == -1)
		return (-1);
	if (oflag & O_WRONLY) // ✅ If in write mode, do not read the file
		return (fd);
	return (read_file_content(fd, file_content));
}

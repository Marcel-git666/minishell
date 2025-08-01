/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manipulation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 21:44:07 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/01 14:01:18 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Appends buffer content to existing file_content string
 * Returns 1 on success, 0 on memory allocation failure
 */
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

/*
 * Creates and opens a file with given flags and mode
 * Handles file creation if O_CREAT flag is set
 * Returns file descriptor on success, -1 on failure
 */
static int	create_and_open_file(char *name, int oflag, mode_t mode)
{
	int	fd;

	fd = open(name, oflag, mode);
	if (fd == -1 && (oflag & O_CREAT))
	{
		fd = open(name, oflag | O_CREAT, mode);
		if (fd == -1)
			return (write(2, "Error: Could not open history file.\n", 36), -1);
	}
	return (fd);
}

/*
 * Reads entire file content into a dynamically allocated string
 * Reads file in 4096-byte chunks and concatenates them
 * Returns 1 on success, -1 on read error, 0 on memory error
 */
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

/*
 * Opens a file and optionally reads its content
 * If O_WRONLY flag is set, returns fd without reading
 * Otherwise reads entire file content into file_content parameter
 * Returns fd for write-only mode, 1 for successful read, -1 for errors
 */
int	open_file(char *name, char **file_content, int oflag)
{
	int	fd;

	fd = create_and_open_file(name, oflag, 0644);
	if (fd == -1)
		return (-1);
	if (oflag & O_WRONLY)
		return (fd);
	return (read_file_content(fd, file_content));
}

/*
 * Generates unique temporary filename using static counter
 * Creates filename in format "tempN.txt" where N increments
 * Returns allocated filename string
 */
char	*new_tempfile(void)
{
	char		*name;
	char		*name_temp;
	char		*num;
	static int	i;

	num = ft_itoa(i++);
	name = ft_strjoin("temp", num);
	name_temp = ft_strjoin(name, ".txt");
	free(num);
	free(name);
	return (name_temp);
}

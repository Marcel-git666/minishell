/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:47:54 by lformank          #+#    #+#             */
/*   Updated: 2025/07/20 13:55:39 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*
 * Closes and restores file descriptors based on redirection type
 * Restores original stdin/stdout from saved descriptors
 */
void	close_fd(t_fds *fd, enum e_redir_type type)
{
	if (fd->out_new != -1 && type == REDIR_OUT)
	{
		dup2(fd->out_old, 1);
		close(fd->out_old);
	}
	else if (fd->in_new != -1 && type == REDIR_IN)
	{
		dup2(fd->in_old, 0);
		close(fd->in_old);
	}
}

/*
 * Opens file descriptors for redirections based on type
 * Handles input (<), output (>), and append (>>) redirections
 * Closes previous descriptors if already open
 */
int	fd(t_ast_node *ast, t_fds *fd, enum e_redir_type type)
{
	if (type == REDIR_OUT || type == REDIR_APPEND)
	{
		if (fd->out_new != -1)
			close(fd->out_new);
		if (type == REDIR_OUT)
			fd->out_new = open(ast->u_content.s_redir.redir->file_or_delimiter,
					O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else
			fd->out_new = open(ast->u_content.s_redir.redir->file_or_delimiter,
					O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd->out_new == -1)
			return (perror("minishell"), -1);
	}
	else if (type == REDIR_IN)
	{
		if (fd->in_new != -1)
			close(fd->in_new);
		fd->in_new = open(ast->u_content.s_redir.redir->file_or_delimiter,
				O_RDONLY, 0666);
		if (fd->in_new == -1)
			return (perror("minishell"), -1);
	}
	return (0);
}

/*
 * Allocates and initializes file descriptor structure
 * Sets all descriptors to -1 (invalid) and temp pointer to NULL
 * Returns allocated structure or NULL on failure
 */
t_fds	*set_fd(void)
{
	t_fds	*fd;

	fd = malloc(1 * sizeof(t_fds));
	if (!fd)
		return (NULL);
	fd->in_new = -1;
	fd->in_old = -1;
	fd->out_new = -1;
	fd->out_old = -1;
	fd->here_new = -1;
	fd->temp = NULL;
	return (fd);
}

/*
 * Resets all file descriptors and cleans up resources
 * Restores original stdin/stdout, closes all descriptors, removes temp files
 * Frees the entire fd structure
 */
void	reset_fd(t_fds *fd)
{
	if (!fd)
		return ;
	if (fd->out_old != -1)
	{
		dup2(fd->out_old, 1);
		close(fd->out_old);
	}
	if (fd->in_old != -1)
	{
		dup2(fd->in_old, 0);
		close(fd->in_old);
	}
	if (fd->out_new != -1)
		close(fd->out_new);
	if (fd->in_new != -1)
		close(fd->in_new);
	if (fd->temp)
	{
		unlink(fd->temp);
		free(fd->temp);
	}
	free(fd);
}

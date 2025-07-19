/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:47:54 by lformank          #+#    #+#             */
/*   Updated: 2025/07/20 00:31:13 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	}
	else if (type == REDIR_IN)
	{
		if (fd->in_new != -1)
			close(fd->in_new);
		fd->in_new = open(ast->u_content.s_redir.redir->file_or_delimiter,
				O_RDONLY, 0666);
	}
	return (0);
}

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:47:54 by lformank          #+#    #+#             */
/*   Updated: 2025/08/01 14:00:09 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"

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
 * Handles file redirection based on type (input, output, append)
 * Opens appropriate file descriptor with correct flags
 * Returns 0 on success, -1 on failure
 */
static int	handle_file_redirect(t_fds *fd, char *filename,
		enum e_redir_type type)
{
	if (type == REDIR_OUT || type == REDIR_APPEND)
	{
		if (fd->out_new != -1)
			close(fd->out_new);
		if (type == REDIR_OUT)
			fd->out_new = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else
			fd->out_new = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd->out_new == -1)
			return (perror("minishell"), -1);
	}
	else if (type == REDIR_IN)
	{
		if (fd->in_new != -1)
			close(fd->in_new);
		fd->in_new = open(filename, O_RDONLY, 0666);
		if (fd->in_new == -1)
			return (perror("minishell"), -1);
	}
	return (0);
}

/*
 * Opens file descriptors for redirections based on type
 * Expands filename variables and delegates to specific handlers
 * Returns 0 on success, -1 on failure
 */
int	fd(t_ast_node *ast, t_fds *fd, enum e_redir_type type, t_shell *shell)
{
	char	*expanded_filename;
	int		result;

	expanded_filename = expand_variables(
			ast->u_content.s_redir.redir->file_or_delimiter,
			shell->env, shell->last_exit_code, 0);
	if (!expanded_filename)
		return (-1);
	if (type == REDIR_OUT || type == REDIR_APPEND || type == REDIR_IN)
		result = handle_file_redirect(fd, expanded_filename, type);
	else
		result = 0;
	free(expanded_filename);
	return (result);
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

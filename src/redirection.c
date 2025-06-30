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

char	*find_heredocs(t_ast_node *ast)
{
	char		*delimiter;

	delimiter = NULL;
	while (ast && ast->type == NODE_REDIR)
	{
		if (ast->u_content.redir.redir->type == REDIR_HEREDOC)
		{
			if (delimiter != NULL)
				free(delimiter);
			delimiter = ft_strdup(ast->u_content.redir.redir->file_or_delimiter);
		}
		ast = ast->u_content.redir.child;
	}
	return (delimiter);
}

void	heredoc(t_ast_node *ast_node, t_fds *fd)
{
	char		*input;
	char		*delimiter;

	delimiter = find_heredocs(ast_node);
	fd->temp_file = new_tempfile();
	fd->here_new = open(fd->temp_file, O_CREAT | O_RDWR | O_TRUNC);
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delimiter) == 0)
		{
			if (input)
				free(input);
			break;
		}
		write(fd->here_new, input, ft_strlen(input));
		write(fd->here_new, "\n", 1);
		free(input);
	}
	close(fd->here_new);
	ast_node->u_content.redir.redir->type = REDIR_IN;
	free(ast_node->u_content.redir.redir->file_or_delimiter);
	ast_node->u_content.redir.redir->file_or_delimiter = ft_strdup(fd->temp_file);
	free(fd->temp_file);
	fd->temp_file = NULL;
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

void	save_old(t_ast_node *ast, t_fds *fd)
{
	if (ast->u_content.redir.redir->type == REDIR_OUT)
		fd->out_old = dup(STDOUT_FILENO);
	else if (ast->u_content.redir.redir->type == REDIR_IN)
		fd->in_old = dup(STDIN_FILENO);
}

int	fd(t_ast_node *ast, t_fds *fd, enum e_redir_type type)
{
	if (type == REDIR_OUT || type == REDIR_APPEND)
	{
		if (fd->out_new != -1)
			close(fd->out_new);
		if (type == REDIR_OUT)
			fd->out_new = open(ast->u_content.redir.redir->file_or_delimiter,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else
			fd->out_new = open(ast->u_content.redir.redir->file_or_delimiter,
				O_WRONLY | O_CREAT | O_APPEND, 0666);
	}
	else if (type == REDIR_IN)
	{
		if (fd->in_new != -1)
			close(fd->in_new);
		fd->in_new = open(ast->u_content.redir.redir->file_or_delimiter,
			O_RDONLY, 0666);
	}
	return (0);
}

int	redirection(t_ast_node *ast_node, t_fds *fd_)
{
	save_old(ast_node, fd_);
	heredoc(ast_node, fd_);
	while (ast_node && ast_node->type == NODE_REDIR)
	{
		if (fd(ast_node, fd_, ast_node->u_content.redir.redir->type) == -1)
			return (-1);
		ast_node = ast_node->u_content.redir.child;
	}
	if (fd_->out_new != -1)
		dup2(fd_->out_new, 1);
	if (fd_->in_new != -1)
		dup2(fd_->in_new, 0);
	return (0);
}

void	set_fd(t_fds *fd)
{
	fd->in_new = -1;
	fd->in_old = -1;
	fd->out_new = -1;
	fd->out_old = -1;
	fd->here_new = -1;
}

void	reset_fd(t_fds *fd)
{
	if (!fd)
		return ;
	if (fd->out_old != -1)
	{
		dup2(fd->out_old, 1);
		close(fd->out_old);
		if (fd->out_new)
			close(fd->out_new);
	}
	if (fd->in_old != -1)
	{
		dup2(fd->in_old, 0);
		close(fd->in_old);
		if (fd->in_new)
			close(fd->in_new);
	}
	free(fd);
}

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
	free(name);
	free(num);
	return (name_temp);
}

void	heredoc(t_ast_node *ast_node, t_fds *fd)
{
	char	*input;
	char	*name_temp;

	// temporary memory for all lines of heredoc
	name_temp = new_tempfile();
	fd->here_new = open(name_temp, O_CREAT | O_RDWR | O_TRUNC);
	input = readline("> ");
	while (input && ft_strcmp(input,
		ast_node->u_content.redir.redir->file_or_delimiter))
	{
		write(fd->here_new, input, ft_strlen(input));
		write(fd->here_new, "\n", 1);
		free(input);
		input = readline("> ");
	}
	free(input);
	close(fd->here_new);
	ast_node->u_content.redir.redir->type = REDIR_IN;
	free(ast_node->u_content.redir.redir->file_or_delimiter);
	ast_node->u_content.redir.redir->file_or_delimiter = ft_strdup(name_temp);
	free(name_temp);
}

void	close_fd(t_fds *fd, enum e_redir_type type)
{
	printf("new: %d: %d\n", fd->out_new, type);
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
	else if (fd->append_new != -1 && type == REDIR_APPEND)
	{
		dup2(fd->append_old, 1);
		close(fd->append_old);
	}
	// if (fd->out_old > -1 && type == REDIR_OUT)
	// {
	// 	dup2(fd->out_old, 1);
	// 	close(fd->out_old);
	// }
	// else if (fd->in_old > -1 && type == REDIR_IN)
	// {
	// 	dup2(fd->in_old, 0);
	// 	close(fd->in_old);
	// }
	// else if (fd->append_old > -1 && type == REDIR_APPEND)
	// {
	// 	dup2(fd->append_old, 1);
	// 	close(fd->append_old);
	// }
}

void	fd(t_ast_node *ast, t_fds *fd, enum e_redir_type type)
{
	printf("out: %d, in: %d, append: %d\n", fd->out_new, fd->in_new, fd->append_new);
	if (fd->out_new != -1 || fd->in_new != -1 || fd->append_new != -1)
		close_fd(fd, type);
	if (type == REDIR_OUT)
		fd->out_old = dup(STDOUT_FILENO);
	else if (type == REDIR_APPEND)
		fd->append_old = dup(STDOUT_FILENO);
	else if (type == REDIR_IN)
		fd->in_old = dup(STDIN_FILENO);
	if (type == REDIR_OUT)
		fd->out_new = open(ast->u_content.redir.redir->file_or_delimiter,
			O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (type == REDIR_IN)
		fd->in_new = open(ast->u_content.redir.redir->file_or_delimiter,
			O_RDONLY, 0666);
	else if (type == REDIR_APPEND)
		fd->append_new = open(ast->u_content.redir.redir->file_or_delimiter,
			O_APPEND | O_WRONLY | O_CREAT, 0666);
	if (type == REDIR_OUT && fd->out_new > -1)
		dup2(fd->out_new, 1);
	else if (type == REDIR_IN && fd->in_new > -1)
		dup2(fd->in_new, 0);
	else if (type == REDIR_APPEND && fd->append_new > -1)
		dup2(fd->append_new, 1);
}

void	redirection(t_ast_node *ast_node, t_fds *fd_)
{
	while (ast_node->type == NODE_REDIR)
	{
		printf("%u:%s\n", ast_node->u_content.redir.redir->type, ast_node->u_content.redir.redir->file_or_delimiter);
		if (ast_node->u_content.redir.redir->type == REDIR_HEREDOC)
			heredoc(ast_node, fd_);
		if (ast_node->u_content.redir.redir->type == REDIR_OUT)
			fd(ast_node, fd_, ast_node->u_content.redir.redir->type);
		else if (ast_node->u_content.redir.redir->type == REDIR_IN)
			fd(ast_node, fd_, ast_node->u_content.redir.redir->type);
		else if (ast_node->u_content.redir.redir->type == REDIR_APPEND)
			fd(ast_node, fd_, ast_node->u_content.redir.redir->type);
		if (ast_node->u_content.redir.child)
		{
			ast_node = ast_node->u_content.redir.child;
			if (ast_node->type == NODE_REDIR)
				if (fd_->out_new != -1 || fd_->in_new != -1 || fd_->append_new != -1)
					close_fd(fd_, ast_node->u_content.redir.redir->type);
		}
		else
			return ;
		// if (fd_->out_new != -1 || fd_->in_new != -1 || fd_->append_new != -1)
		// 	close_fd(fd_, ast_node->u_content.redir.redir->type);
	}
}

void	set_fd(t_fds *fd)
{
	fd->in_new = -1;
	fd->in_old = -1;
	fd->out_new = -1;
	fd->out_old = -1;
	fd->append_new = -1;
	fd->append_old = -1;
	fd->here_new = -1;
}

void	reset_fd(t_fds *fd)
{
	if (fd->append_old > -1)
	{
		dup2(fd->append_old, 1);
		close(fd->append_old);
	}
	if (fd->out_old > -1)
	{
		dup2(fd->out_old, 1);
		close(fd->out_old);
	}
	if (fd->in_old > -1)
	{
		dup2(fd->in_old, 0);
		close(fd->in_old);
	}
	free(fd);
}

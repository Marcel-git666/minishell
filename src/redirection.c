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

int	heredoc(t_ast_node *ast_node, t_fds *fd)
{
	char	*newline;
	char	*delimiter;
	int		pid;
	int		status;

	pid = -1;
	delimiter = find_heredocs(ast_node);
	fd->temp_file = ft_strdup("temp.txt");
	if (delimiter)
		pid = fork();
	else
		return (0);
	if (pid == -1)
	{
		free(delimiter);
		return (-1);
	}
	else if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		free(delimiter);
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		{
			ast_node->u_content.redir.redir->type = REDIR_IN;
			free(ast_node->u_content.redir.redir->file_or_delimiter);
			ast_node->u_content.redir.redir->file_or_delimiter = ft_strdup(fd->temp_file);
			return (0);
		}
		else
		{
			unlink(fd->temp_file);
			return (-1);
		}
	}
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_DFL);
	fd->here_new = open(fd->temp_file, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd->here_new == -1)
	{
		perror("minishell: heredoc");
		free(delimiter);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		newline = readline("> ");
		if (!newline || ft_strcmp(newline, delimiter) == 0)
		{
			if (newline)
				free(newline);
			else
				write(fd->here_new, "bash: warning: here-document at line 1 delimited by end-of-file (wanted `EOF')\n", 79);
			break;
		}
		write(fd->here_new, newline, ft_strlen(newline));
		write(fd->here_new, "\n", 1);
		free(newline);
	}
	close(fd->here_new);
	free(delimiter);
	exit(EXIT_SUCCESS);
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

void	save_old(t_fds *fd)
{
	fd->out_old = dup(STDOUT_FILENO);
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
	save_old(fd_);
	if (heredoc(ast_node, fd_) == -1)
		return (-1);
	while (ast_node && ast_node->type == NODE_REDIR)
	{
		printf("in: %d, out: %d\n", fd_->in_new, fd_->out_new);
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
	fd->temp_file = NULL;
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
	if (fd->temp_file)
	{
		unlink(fd->temp_file);
		free(fd->temp_file);
	}
	free(fd);
}

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

/*
 * Finds and returns the last heredoc delimiter in AST chain
 * Traverses redirection nodes to find REDIR_HEREDOC type
 * Returns duplicated delimiter string or NULL if none found
 */
char	*find_heredocs(t_ast_node *ast)
{
	char		*delim;

	delim = NULL;
	while (ast && ast->type == NODE_REDIR)
	{
		if (ast->u_content.s_redir.redir->type == REDIR_HEREDOC)
		{
			if (delim != NULL)
				free(delim);
			delim = ft_strdup(ast->u_content.s_redir.redir->file_or_delimiter);
		}
		ast = ast->u_content.s_redir.child;
	}
	return (delim);
}

/*
 * Handles parent process after heredoc child completes
 * Waits for child, converts heredoc to input redirection on success
 * Returns 0 on success, -1 on failure
 */
int	parent(t_ast_node *ast, char *delimiter, int pid, t_fds *fd)
{
	int		status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	free(delimiter);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
	{
		ast->u_content.s_redir.redir->type = REDIR_IN;
		free(ast->u_content.s_redir.redir->file_or_delimiter);
		ast->u_content.s_redir.redir->file_or_delimiter = ft_strdup(fd->temp);
		return (0);
	}
	else
	{
		unlink(fd->temp);
		return (-1);
	}
}

/*
 * Reads heredoc input from user until delimiter is encountered
 * Creates temporary file and writes input lines to it
 * Handles EOF and delimiter matching for termination
 */
void	read_loop(char *delimiter, t_fds *fd)
{
	char	*newline;

	fd->here_new = open(fd->temp, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd->here_new == -1)
	{
		perror("minishell: heredoc\n");
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
				write(fd->here_new, "bash: warning: here-document at line 1\
delimited by end-of-file (wanted `EOF')\n", 79);
			break ;
		}
		write(fd->here_new, newline, ft_strlen(newline));
		write(fd->here_new, "\n", 1);
		free(newline);
	}
}

/*
 * Implements heredoc functionality (<<) by forking child process
 * Child process reads input, parent waits and processes result
 * Returns 0 on success, -1 on failure
 */
int	heredoc(t_ast_node *ast_node, t_fds *fd)
{
	char	*delimiter;
	int		pid;

	delimiter = find_heredocs(ast_node);
	fd->temp = ft_strdup("temp.txt");
	if (delimiter)
		pid = fork();
	else
		return (0);
	if (pid == -1)
		return (-1);
	else if (pid > 0)
	{
		if (parent(ast_node, delimiter, pid, fd) == 0)
			return (0);
		else
			return (-1);
	}
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
	read_loop(delimiter, fd);
	close(fd->here_new);
	free(delimiter);
	exit(EXIT_SUCCESS);
}

/*
 * Main redirection handler - processes all redirection types
 * Saves original stdin/stdout, handles heredocs, applies redirections
 * Returns 0 on success, -1 on failure
 */
int	redirection(t_ast_node *ast_node, t_fds *fd_)
{
	fd_->out_old = dup(STDOUT_FILENO);
	fd_->in_old = dup(STDIN_FILENO);
	if (heredoc(ast_node, fd_) == -1)
		return (-1);
	while (ast_node && ast_node->type == NODE_REDIR)
	{
		if (fd(ast_node, fd_, ast_node->u_content.s_redir.redir->type) == -1)
			return (-1);
		ast_node = ast_node->u_content.s_redir.child;
	}
	if (fd_->out_new != -1)
		dup2(fd_->out_new, 1);
	if (fd_->in_new != -1)
		dup2(fd_->in_new, 0);
	return (0);
}

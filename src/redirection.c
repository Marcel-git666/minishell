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

void	heredoc(t_ast_node *ast_node, int *newfd)
{
	char	*input;
	char	*name_temp;

	// temporary memory for all lines of heredoc
	name_temp = new_tempfile();
	*newfd = open(name_temp, O_CREAT | O_RDWR | O_TRUNC);
	input = readline("> ");
	while (input && ft_strcmp(input,
		ast_node->u_content.redir.redir->file_or_delimiter))
	{
		write(*newfd, input, ft_strlen(input));
		write(*newfd, "\n", 1);
		free(input);
		input = readline("> ");
	}
	free(input);
	close(*newfd);
	ast_node->u_content.redir.redir->type = REDIR_IN; // change heredoc to in
	free(ast_node->u_content.redir.redir->file_or_delimiter);
	ast_node->u_content.redir.redir->file_or_delimiter = ft_strdup(name_temp);
	free(name_temp);
}

void	closing(int out, int in, int append, int *oldfd)
{
	if (out > 1)
	{
		dup2(*oldfd, 1);
		close(*oldfd);
	}
	else if (in > 1)
	{
		dup2(*oldfd, 0);
		close(*oldfd);
	}
	else if (append > 1)
	{
		dup2(*oldfd, 1);
		close(*oldfd);
	}
	// if (out > 1 || in > 1 || append > 1)
	// {
	// 	close(*oldfd);
	// 	*oldfd = 0;
	// }
}

void	fd(t_ast_node *ast, int *newfd, int *oldfd, enum e_redir_type type)
{
	int			default_fd;
	static int	in;
	static int	out;
	static int	append;

	default_fd = -1;
	if (type == REDIR_OUT || type == REDIR_APPEND)
		default_fd = 1;
	else if (type == REDIR_IN)
		default_fd = 0;
	*oldfd = dup(default_fd);
	if (type == REDIR_OUT)
	{
		*newfd = open(ast->u_content.redir.redir->file_or_delimiter,
			O_TRUNC | O_CREAT | O_WRONLY);
		out++;
	}
	else if (type == REDIR_IN)
	{
		*newfd = open(ast->u_content.redir.redir->file_or_delimiter, O_RDONLY);
		in++;
	}
	else if (type == REDIR_APPEND)
	{
		*newfd = open(ast->u_content.redir.redir->file_or_delimiter,
			O_APPEND | O_WRONLY | O_CREAT);
		append++;
	}
	if (out > 1 || in > 1 || append > 1)
		closing(out, in, append, oldfd);
	if (*newfd > -1)
		dup2(*newfd, default_fd);
}

void	redirection(t_ast_node *ast_node, int *newfd, int *oldfd)
{
	while (ast_node->type == NODE_REDIR)
	{
		if (ast_node->u_content.redir.redir->type == REDIR_HEREDOC)
			heredoc(ast_node, newfd);
		if (ast_node->u_content.redir.redir->type == REDIR_OUT)
			fd(ast_node, newfd, oldfd, ast_node->u_content.redir.redir->type);
		else if (ast_node->u_content.redir.redir->type == REDIR_IN)
			fd(ast_node, newfd, oldfd, ast_node->u_content.redir.redir->type);
		else if (ast_node->u_content.redir.redir->type == REDIR_APPEND)
			fd(ast_node, newfd, oldfd, ast_node->u_content.redir.redir->type);
		if (ast_node->u_content.redir.child)
			ast_node = ast_node->u_content.redir.child;
		else
			return ;
	}
}

void	reset_fd(int *oldfd, t_ast_node *orig)
{
	if (oldfd)
	{
		if (orig->u_content.redir.redir->type == REDIR_OUT || 
			orig->u_content.redir.redir->type == REDIR_APPEND)
			dup2(*oldfd, 1);
		else if (orig->u_content.redir.redir->type == REDIR_IN)
			dup2(*oldfd, 0);
		close(*oldfd);
	}
}

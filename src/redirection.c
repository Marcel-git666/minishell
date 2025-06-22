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

void	redirection(t_ast_node *ast_node, int *newfd, int *oldfd)
{
	if (ast_node->u_content.redir.redir->type == REDIR_OUT)
	{
		while (ast_node->u_content.redir.child)
		{
			ast_node = ast_node->u_content.redir.child;
			if (ast_node->type != 0 && ast_node->u_content.redir.redir->type != REDIR_OUT)
				break;
			*oldfd = dup(1);
			*newfd = open(ast_node->u_content.redir.redir->file_or_delimiter, O_WRONLY | O_CREAT | O_TRUNC);
			if (ast_node->type != 0 && ast_node->u_content.redir.redir->type != REDIR_OUT)
			{
				close (*newfd);
				return ;
			}
			if (*newfd > -1)
				dup2(*newfd, 1);
		}
	}
	else if (ast_node->u_content.redir.redir->type == REDIR_IN)
	{
		*oldfd = dup(0);
		*newfd = open(ast_node->u_content.redir.redir->file_or_delimiter, O_RDONLY);
		if (*newfd > -1)
			dup2(*newfd, 0);
	}
	else if (ast_node->u_content.redir.redir->type == REDIR_APPEND)
	{
		*oldfd = dup(1);
		*newfd = open(ast_node->u_content.redir.redir->file_or_delimiter, O_APPEND | O_WRONLY |O_CREAT);
		if (*newfd > -1)
			dup2(*newfd, 1);
	}
	// if (ast_node)
	// else if (ast_node->u_content.redir.redir->type == REDIR_HEREDOC)
	// {

	// }

}

void	reset_fd(int *oldfd, t_ast_node *orig)
{
	if (oldfd)
	{
		if (orig->u_content.redir.redir->type == REDIR_OUT)
			dup2(*oldfd, 1);
		else if (orig->u_content.redir.redir->type == REDIR_IN)
			dup2(*oldfd, 0);
		else if (orig->u_content.redir.redir->type == REDIR_APPEND)
			dup2(*oldfd, 1);
		close(*oldfd);
	}
}

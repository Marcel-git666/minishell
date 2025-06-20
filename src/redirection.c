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
		*oldfd = dup(1);
		*newfd = open(ast_node->u_content.redir.redir->file_or_delimiter, O_WRONLY);
		if (*newfd > -1)
			dup2(*newfd, 1);
	}
	else if (ast_node->u_content.redir.redir->type == REDIR_IN)
	{
		*oldfd = dup(0);
		*newfd = open(ast_node->u_content.redir.redir->file_or_delimiter, O_RDONLY);
		if (*newfd > -1)
			dup2(*newfd, 0);
	}
}

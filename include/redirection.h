/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:03:05 by lformank          #+#    #+#             */
/*   Updated: 2025/06/04 14:04:04 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "minishell.h"

typedef struct s_fds
{
	int		in_new;
	int		in_old;
	int		out_new;
	int 	out_old;
	int 	here_new;
	char	*temp_file;
} t_fds;

int		redirection(t_ast_node *ast_node, t_fds *fd_);
void	reset_fd(t_fds *fd);

void	heredoc(t_ast_node *ast_node, t_fds *fd);
void	set_fd(t_fds *fd);
void	save_old(t_ast_node *ast, t_fds *fd);

#endif
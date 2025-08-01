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
	int		out_old;
	int		here_new;
	char	*temp;
}	t_fds;

/*redirections*/
int		redirection(t_ast_node *ast_node, t_fds *fd_, t_shell *shell);

/*heredoc*/
char	*find_heredocs(t_ast_node *ast);
int		parent(t_ast_node *ast, char *delimiter, int pid, t_fds *fd);
void	read_loop(char *delimiter, t_fds *fd);
int		heredoc(t_ast_node *ast_node, t_fds *fd);

/*signals*/
void	signal_handler_heredoc(int signum);

#endif
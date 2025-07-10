/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:50:07 by lformank          #+#    #+#             */
/*   Updated: 2025/07/06 23:06:37 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FD_H
# define FD_H

# include "minishell.h"

char	*new_tempfile(void);
void	close_fd(t_fds *fd, enum e_redir_type type);
int		fd(t_ast_node *ast, t_fds *fd, enum e_redir_type type);
t_fds	*set_fd(void);
void	reset_fd(t_fds *fd);

#endif
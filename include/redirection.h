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

// Forward declarations
struct s_ast_node;
typedef struct s_ast_node t_ast_node;
struct s_shell;
typedef struct s_shell t_shell;

// Main redirection function
void redirection(t_ast_node *root, t_shell *shell, char **envp);

// Individual redirection handlers
int handle_input_redirection(char *filename);
int handle_output_redirection(char *filename);
int handle_append_redirection(char *filename);
int handle_heredoc(char *delimiter);
int	setup_redirection(t_redirection *redir, int *saved_stdin,
				int *saved_stdout);


#endif
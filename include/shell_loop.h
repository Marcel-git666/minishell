/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:55:08 by marcel            #+#    #+#             */
/*   Updated: 2025/06/06 11:40:27 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_LOOP_H
# define SHELL_LOOP_H

struct s_shell;
typedef struct s_shell t_shell;

// shell_loop.c
t_shell		*initialize_shell(char **envp);
void		handle_input(char *input);
void		run_shell_loop(t_shell *shell, char **envp);

#endif
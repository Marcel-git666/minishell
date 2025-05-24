/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:55:08 by marcel            #+#    #+#             */
/*   Updated: 2025/05/18 22:06:42 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_LOOP_H
# define SHELL_LOOP_H

struct s_shell;
typedef struct s_shell t_shell;

// shell_loop.c
t_shell		*initialize_shell(char **envp);
void		handle_input(char *input);
void		run_shell_loop(t_shell *shell);

#endif
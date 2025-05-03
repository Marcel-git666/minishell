/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:55:08 by marcel            #+#    #+#             */
/*   Updated: 2025/05/03 18:55:38 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_LOOP_H
# define SHELL_LOOP_H

// shell_loop.c
void		initialize_shell(char **envp);
void		handle_input(char *input);
void		run_shell_loop(void);

#endif
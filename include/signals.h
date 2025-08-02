/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:53:45 by marcel            #+#    #+#             */
/*   Updated: 2025/08/02 10:04:56 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H   
# define SIGNALS_H

// signals.c
void		setup_signals(void);
void		signal_handler(int signum);

#endif
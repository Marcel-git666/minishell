/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 01:57:05 by marcel            #+#    #+#             */
/*   Updated: 2025/05/04 02:12:57 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// Forward declaration
struct s_lexer;
typedef struct s_lexer t_lexer;

// utils.c
int			ft_isspace(int c);
char		*ft_strndup(const char *s, size_t n);
void		error_message(const char *error);
int			check_next_token(t_lexer *lexer, size_t next_pos);

#endif
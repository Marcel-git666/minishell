/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:25:28 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/17 17:22:06 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

void	error_message(const char *error)
{
	write(2, "minishell: ", 11);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*copy;

	copy = (char *)malloc(n + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

int	check_next_token(t_lexer *lexer, size_t next_pos)
{
	while (lexer->input[next_pos] && ft_isspace(lexer->input[next_pos]))
		next_pos++;
	if (!lexer->input[next_pos] || lexer->input[next_pos] == '>'
		|| lexer->input[next_pos] == '<' || lexer->input[next_pos] == '|')
		return (error_message("syntax error: redirection requires a filename"),
			-1);
	return (0);
}

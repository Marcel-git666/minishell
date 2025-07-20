/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:25:28 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 17:40:48 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Checks if character is a whitespace character
 * Returns 1 for space, tab, newline, vertical tab, form feed, carriage return
 */
int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

/*
 * Prints error message to stderr with minishell prefix
 * Formats: "minishell: [error]\n"
 */
void	error_message(const char *error)
{
	write(2, "minishell: ", 11);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

/*
 * Prints command not found error message
 * Formats: "minishell: command: command not found"
 */
void	print_command_not_found(char *command)
{
	write(2, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, ": command not found\n", 20);
}

/*
 * Creates duplicate of string with maximum length n
 * Allocates memory and copies up to n characters
 * Returns allocated string or NULL on failure
 */
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

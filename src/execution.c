/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:03:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/03/06 18:41:03 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_token *tokens)
{
	if (!tokens)
		return ;

	if (tokens->type == TOKEN_CMD
		&& ft_strncmp(tokens->value, "exit", 5) == 0)
	{
		free_tokens(tokens);
		builtin_exit();
	}
}

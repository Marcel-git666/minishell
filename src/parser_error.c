/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 22:30:09 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 00:05:44 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * Sets parser error state and stores error message
 * Frees any existing error message before setting new one
 * Returns -1 for convenient error handling in parser functions
 */
int	set_parser_error(t_parser *parser, const char *msg)
{
	parser->error = 1;
	if (parser->error_msg)
		free(parser->error_msg);
	parser->error_msg = ft_strdup(msg);
	return (-1);
}

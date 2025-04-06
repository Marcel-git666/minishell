/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsre_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:45:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/04/06 09:46:17 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	free_ast(t_ast_node *node)
{
	int		i;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		if (node->u_content.cmd.cmd)
			free(node->u_content.cmd.cmd);
		if (node->u_content.cmd.args)
		{
			i = -1;
			while (++i < node->u_content.cmd.arg_count)
				free(node->u_content.cmd.args[i]);
			free(node->u_content.cmd.args);
		}
	}
	free(node);
}

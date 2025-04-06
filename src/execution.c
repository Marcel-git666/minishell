/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:03:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/04/05 23:38:01 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	execute_command(t_ast_node *ast_node)
{
	if (!ast_node)
		return ;
	if (ast_node->type == NODE_COMMAND)
	{
		if (ft_strncmp(ast_node->u_content.cmd.cmd, "exit", 5) == 0)
			builtin_exit();
		else
		{
			// For now, just print command info
			printf("Would execute: %s", ast_node->u_content.cmd.cmd);
			if (ast_node->u_content.cmd.arg_count > 0)
			{
				printf(" with args:");
				for (int i = 0; i < ast_node->u_content.cmd.arg_count; i++)
					printf(" %s", ast_node->u_content.cmd.args[i]);
			}
			printf("\n");
		}
	}
}

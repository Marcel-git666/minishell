/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:03:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 17:01:45 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "env.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void	search_command(/*t_ast_node *ast_node, t_shell *shell*/)
{
	DIR				*direc;
	struct dirent	*dir;
	struct stat		statbuf;
	char			*path;

	direc = opendir(".");
	dir = readdir(direc);
	while (direc && dir)
	{
		printf("%s\n", dir->d_name);
		path = ft_strjoin("./", dir->d_name);
		if (stat(path, &statbuf) == 0)
		{
			if (S_ISDIR(statbuf.st_mode)) // check inode num if file is directory
				printf("\t%s\n", path);
		}
		dir = readdir(direc);
	}
	free(path);
	closedir(direc);
}

void	execute_command(t_ast_node *ast_node, t_shell *shell)
{
	if (!ast_node)
		return ;
	if (ast_node->type == NODE_REDIR)
	{
		print_ast(ast_node, 0);
		print_ast(ast_node->u_content.redir.child, 0);
	}
	if (ast_node->type == NODE_COMMAND)
	{
		if (ft_strncmp(ast_node->u_content.cmd.cmd, "exit", 5) == 0)
			builtin_exit();
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "env", 4) == 0)
			env_print(shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "pwd", 4) == 0)
			builtin_pwd(shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "cd", 3) == 0)
			builtin_cd(ast_node, shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "export", 7) == 0)
			builtin_export(ast_node, shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "unset", 6) == 0)
			builtin_unset(ast_node, shell->env);
		else if (ft_strncmp(ast_node->u_content.cmd.cmd, "echo", 5) == 0)
			builtin_echo(ast_node);
		else
		{
			search_command(/*ast_node, shell->env*/);
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

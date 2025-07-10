/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:56:35 by mmravec           #+#    #+#             */
/*   Updated: 2025/06/04 11:54:09 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "builtins.h"

void	builtin_exit(t_shell *shell, t_fds *fd, t_ast_node *ast)
{
	t_env	*current;
	t_env	*next;

	current = shell->env;
	if (shell->env)
	{
		while (current)
		{
			next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = next;
		}
		shell->env = NULL;
	}
	free_ast(ast);
	free(fd->temp);
	free(fd);
	printf("Exiting minishell...\n");
	shell->last_exit_code = 0; // Set last exit code to 0 before exiting
	exit(0);
}

void	builtin_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		shell->last_exit_code = 0;
	}
	else
	{
		perror("pwd");
		shell->last_exit_code = 1;
	}
}

void	builtin_cd(t_ast_node *root, t_shell *shell)
{
	char	*cwd;
	char	*test_cwd;

	cwd = malloc(PATH_MAX * sizeof(char));
	getcwd(cwd, PATH_MAX);
	if (root->u_content.cmd.arg_count > 1)
	{
		error_message("cd: too many arguments");
		free(cwd);
		shell->last_exit_code = 1;
		return ;
	}
	if (root->u_content.cmd.arg_count == 1 && 
    	ft_strcmp(root->u_content.cmd.args[0], "-") == 0)
	{
    char *oldpwd = env_get(shell->env, "OLDPWD");
    if (!oldpwd)
	{
        error_message("cd: OLDPWD not set\n");
        shell->last_exit_code = 1;
        free(cwd);
        return;
    }
    env_set(&shell->env, "OLDPWD", cwd);
    chdir(oldpwd);
	}
	else if (root->u_content.cmd.arg_count
    	&& ft_strncmp(root->u_content.cmd.args[0], "..", 2) == 0)
    previous_rep(shell->env, cwd);
	else
		path(root, shell->env, cwd);
	free(cwd);
	cwd = malloc(PATH_MAX * sizeof(char));
	getcwd(cwd, PATH_MAX);
	env_set(&shell->env, "PWD", cwd);
	test_cwd = malloc(PATH_MAX * sizeof(char));
	if (getcwd(test_cwd, PATH_MAX) && ft_strcmp(test_cwd, cwd) != 0)
    	shell->last_exit_code = 0;  // CD se povedlo
	else
    	shell->last_exit_code = 1;  // CD selhalo
	free(test_cwd);
	free(cwd);
}

void	builtin_unset(t_ast_node *root, t_shell *shell)
{
	int	i;

	if (root->u_content.cmd.arg_count == 0)
	{
		error_message("unset: not enough arguments");
		shell->last_exit_code = 1;
		return ;
	}
	i = 0;
	while (i < root->u_content.cmd.arg_count)
	{
		env_unset(&shell->env, root->u_content.cmd.args[i]);
		i++;
	}
	shell->last_exit_code = 0;
}

void	builtin_echo(t_ast_node *root, t_shell *shell)
{
	int		i;
	int		newline;

	newline = 1;
	if (root->u_content.cmd.arg_count > 0
		&& ft_strncmp(root->u_content.cmd.args[0], "-n", 2) == 0)
	{
		newline = 0;
		i = 0; // Start from the second argument
	}
	else
		i = -1; // Start from the first argument
	while (++i < root->u_content.cmd.arg_count && root->u_content.cmd.args[i])
	{
		if (root->u_content.cmd.args[i])
		{
			printf("%s", root->u_content.cmd.args[i]);
			// Add space only if not the last argument
			if (i < root->u_content.cmd.arg_count - 1)
				printf(" ");
		}
	}
	if (newline)
		printf("\n");
	shell->last_exit_code = 0;
}

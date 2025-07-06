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

void	builtin_exit(t_shell *shell, t_fds *fd, t_ast_node *ast, char *exp_arg)
{
	t_env *current = shell->env;
	t_env *next;

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
	free(fd->temp_file);
	free(fd);
	free(exp_arg);
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

	cwd = malloc(PATH_MAX * sizeof(char));
	getcwd(cwd, PATH_MAX);
	if (root->u_content.cmd.arg_count > 1)
	{
		printf("~%s\n", cwd);
		free(cwd);
		shell->last_exit_code = 1; 
		return ;
	}
	if (root->u_content.cmd.arg_count
		&& ft_strncmp(root->u_content.cmd.args[0], "..", 2) == 0)
		previous_rep(shell->env, cwd);	
	else
		path(root, shell->env, cwd);
	free(cwd);
	cwd = malloc(PATH_MAX * sizeof(char));
	getcwd(cwd, PATH_MAX);
	env_set(&shell->env, "PWD", cwd);
	free(cwd);
	shell->last_exit_code = 0; 
}

static void handle_export_assignment(char *assignment, t_env **env)
{
    char 	**parts;
    int 	i;
	char	*clean_value;

    parts = ft_split(assignment, '=');
    if (!parts || !parts[0] || !parts[1])
    {
        // Free parts pokud jsou částečně alokovány
        if (parts)
        {
            i = -1;
            while (parts[++i])
                free(parts[i]);
            free(parts);
        }
        return;
    }
	clean_value = parts[1];
	if (clean_value[0] == '"' && clean_value[ft_strlen(clean_value) - 1] == '"')
	{
    	char *temp = ft_substr(clean_value, 1, ft_strlen(clean_value) - 2);
    	env_set(env, parts[0], temp);
    	free(temp);
	}
	else
	    env_set(env, parts[0], clean_value);

    // Free parts
    i = -1;
    while (parts[++i])
        free(parts[i]);
    free(parts);
}

void	builtin_export(t_ast_node *root, t_shell *shell)
{
	if (root->u_content.cmd.arg_count == 1 &&
    	ft_strchr(root->u_content.cmd.args[0], '='))
	{
		handle_export_assignment(root->u_content.cmd.args[0], &shell->env);
		shell->last_exit_code = 0;
    	return ;
	}
	if (root->u_content.cmd.arg_count > 1  || (root->u_content.cmd.arg_count == 1
		&& ft_strncmp(root->u_content.cmd.args[0], "-p", 2) != 0))
	{
		error_message("error: export: -l: invalid option\nexport: usage: export \
or export -p");
		shell->last_exit_code = 1;
		return ;
	}
	t_env *current = shell->env;  // ← Lokální kopie pointeru
    while (current)
    {
        if (root->u_content.cmd.arg_count == 0)
            printf("declare -x %s=%s\n", current->key, current->value);
        current = current->next;  // ← Posuň lokální kopii
    }
	shell->last_exit_code = 0;
}

void	builtin_unset(t_ast_node *root, t_shell *shell)
{
	int	i;

	if (root->u_content.cmd.arg_count == 0)
	{
		error_message("unset: not enough arguments");
		shell->last_exit_code = 1;
		return;
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
	int		newline = 1;

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

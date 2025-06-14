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
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>
#include "expansion.h"

char	*full_path(char **paths, char *path, t_ast_node *ast)
{
	char	*slash;
	int		i;

	i = -1;
	while (paths[++i])
	{
		slash = ft_strjoin(paths[i], "/");
		path = ft_strjoin(slash, ast->u_content.cmd.cmd);
		if (access(path, F_OK) == 0)
		{
			free(slash);
			return (path);
		}
		free(slash);	
		free(path);
	}
	return (NULL);
}
int	fork_it(char *path, char **args, char **envp)
{
	int		pid;
	int		status;

	printf("DEBUG: fork_it called with path='%s'\n", path);
	if (access(path, F_OK) != 0) {
		printf("DEBUG: access() failed for %s\n", path);
		return (-1); 
	}

	pid = fork();
	if (pid == 0) {
		printf("DEBUG: child process executing %s\n", path);
        execve(path, args, envp);
	}
    else if (pid > 0)
    {
		printf("DEBUG: parent waiting for child %d\n", pid);
        wait(&status);
        if (WIFEXITED(status)) {
			printf("DEBUG: child exited with status %d\n", WEXITSTATUS(status));
            return (WEXITSTATUS(status));  // Return actual exit code
		}
		printf("DEBUG: child terminated abnormally\n");
        return (-1);  // Process terminated abnormally
    }
	printf("DEBUG: fork failed\n");
    return (-1); // Fork failed
}

void	free_source(char **path, char *slash, char *final_path, char **args)
{
	int	i;

	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
	free(final_path);
	i = -1;
	while (path[++i])
		free(path[i]);
	free(path);
	free(slash);
}

int	search_command(t_ast_node *ast, t_env *env, char **envp)
{
	char	**paths;
	char	*slash;
	char	*path;
	char	**args;
	int		i;
	int		exit_code;

	printf("DEBUG: search_command called for '%s'\n", ast->u_content.cmd.cmd);
	if (ft_strchr(ast->u_content.cmd.cmd, '/'))
	{
    	printf("DEBUG: Command contains '/', treating as path\n");
		if (access(ast->u_content.cmd.cmd, F_OK) != 0)
    	{
        	printf("DEBUG: File not found: %s\n", ast->u_content.cmd.cmd);
        	return (127);  // Command not found
    	}
    	// Prepare arguments array
    	args = ft_calloc((ast->u_content.cmd.arg_count + 2), sizeof(char *));
    	args[0] = ft_strdup(ast->u_content.cmd.cmd);
    	i = 0;
    	while (i < ast->u_content.cmd.arg_count)
    	{
        	args[i + 1] = ft_strdup(ast->u_content.cmd.args[i]);
        	i++;
    	}
    
    	exit_code = fork_it(ast->u_content.cmd.cmd, args, envp);
    
    	// Free args
    	i = -1;
    	while (args[++i])
        	free(args[i]);
    	free(args);
    
    	return (exit_code);
	}
	i = -1;
	slash = NULL;
	path = NULL;
	while (env && ft_strncmp(env->key, "PATH", 5) != 0)
		env = env->next;
	if (!env) {
        printf("DEBUG: PATH not found in environment!\n");
        return (127);
    }
	printf("DEBUG: PATH = %s\n", env->value);
	paths = ft_split(env->value,':');
	args = ft_calloc((ast->u_content.cmd.arg_count + 2), sizeof(char *));
	args[0] = ft_strdup(ast->u_content.cmd.cmd);
	printf("DEBUG: args[0] = '%s'\n", args[0]);
	while (++i < ast->u_content.cmd.arg_count && ast->u_content.cmd.arg_count != 0)
		args[i + 1] = ft_strdup(ast->u_content.cmd.args[i]);
	path = full_path(paths, path, ast);
	printf("DEBUG: full_path returned: %s\n", path ? path : "NULL");
	if (path)
    {
		printf("DEBUG: Executing %s\n", path);
        exit_code = fork_it(path, args, envp);
		printf("DEBUG: fork_it returned exit_code: %d\n", exit_code);
        free_source(paths, slash, path, args);
        return (exit_code);
    }
	printf("DEBUG: Command not found in PATH\n");
	free_source(paths, slash, path, args);
	return (127); // Command not found
}


void	execute_command(t_ast_node *ast_node, t_shell *shell, char **envp)
{
	char	*expanded_cmd;
	char	*expanded_arg;
	int		i;
	int		exit_code;
	int 	is_env_var;

	i = -1;
	expanded_cmd = NULL;
	if (!ast_node)
		return ;
	if (ast_node->type == NODE_REDIR)
	{
		print_ast(ast_node, 0);
		print_ast(ast_node->u_content.redir.child, 0);
		shell->last_exit_code = 0;  
	}
	if (ast_node->type == NODE_COMMAND)
	{
		expanded_cmd = expand_variables(ast_node->u_content.cmd.cmd, 
                shell->env, shell->last_exit_code, 0);
		if (!expanded_cmd)
		{
			printf("DEBUG: Command expansion failed\n");
			shell->last_exit_code = 1;  
			return ;
		}
		while (++i < ast_node->u_content.cmd.arg_count)
        {
			t_token_type token_type = ast_node->u_content.cmd.arg_token_types[i];
			printf("DEBUG: arg[%d]='%s', token_type=%d\n", i, ast_node->u_content.cmd.args[i], token_type);
    
			if (ast_node->u_content.cmd.arg_token_types[i] == TOKEN_SINGLE_QUOTED) 
				continue; // Single quoted args are not expanded)
			is_env_var = (token_type == TOKEN_ENV_VAR || token_type == TOKEN_EXIT_CODE);
            expanded_arg = expand_variables(ast_node->u_content.cmd.args[i],
                    shell->env, shell->last_exit_code, 
        			is_env_var);
            free(ast_node->u_content.cmd.args[i]);
            ast_node->u_content.cmd.args[i] = expanded_arg;
        }

		if (ft_strcmp(expanded_cmd, "exit") == 0)
		{
			printf("DEBUG: Found exit builtin\n");
			builtin_exit(shell);
		}

		else if (ft_strcmp(expanded_cmd, "env") == 0)
			env_print(shell);
		else if (ft_strcmp(expanded_cmd, "pwd") == 0)
			builtin_pwd(shell);
		else if (ft_strcmp(expanded_cmd, "cd") == 0)
			builtin_cd(ast_node, shell);
		else if (ft_strcmp(expanded_cmd, "export") == 0)
			builtin_export(ast_node, shell);
		else if (ft_strcmp(expanded_cmd, "unset") == 0)
			builtin_unset(ast_node, shell);
		else if (ft_strcmp(expanded_cmd, "echo") == 0)
			builtin_echo(ast_node, shell);
		else
		{
			printf("DEBUG: No builtin found, executing as external command\n");
			exit_code = search_command(ast_node, shell->env, envp);
    		shell->last_exit_code = exit_code;
		}
	}
	free(expanded_cmd);
	return ;
}

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
#include "pipes.h"

char	*full_path(char **paths, char *path, char *cmd)
{
	char	*slash;
	int		i;

	i = -1;
	while (paths[++i])
	{
		slash = ft_strjoin(paths[i], "/");
		path = ft_strjoin(slash, cmd);
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

	if (access(path, F_OK) != 0) {
		return (127); 
	}
	pid = fork();
	if (pid == 0) {
        if (execve(path, args, envp) == -1)
			perror("error: execve failed");
        exit(127);
	}
    else if (pid > 0)
    {
        wait(&status);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));  // Return actual exit code
        return (-1);  // Process terminated abnormally
    }
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

int	search_command(char *expanded_cmd, t_ast_node *ast, t_env *env, char **envp)
{
	char	**paths;
	char	*slash;
	char	*path;
	char	**args;
	int		i;
	int		exit_code;

	if (ft_strchr(expanded_cmd, '/'))
	{
		if (access(expanded_cmd, F_OK) != 0)
    	{
        	return (127);  // Command not found
		}
    	// Prepare arguments array
    	args = ft_calloc((ast->u_content.cmd.arg_count + 2), sizeof(char *));
    	args[0] = ft_strdup(expanded_cmd);
    	i = 0;
    	while (i < ast->u_content.cmd.arg_count)
    	{
        	args[i + 1] = ft_strdup(ast->u_content.cmd.args[i]);
        	i++;
    	}
    
    	exit_code = fork_it(expanded_cmd, args, envp);
    
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
	if (!env)
        return (127);
	paths = ft_split(env->value,':');
	args = ft_calloc((ast->u_content.cmd.arg_count + 2), sizeof(char *));
	args[0] = ft_strdup(expanded_cmd);
	while (++i < ast->u_content.cmd.arg_count && ast->u_content.cmd.arg_count != 0)
		args[i + 1] = ft_strdup(ast->u_content.cmd.args[i]);
	path = full_path(paths, path, expanded_cmd);
	if (path)
    {
        exit_code = fork_it(path, args, envp);
        free_source(paths, slash, path, args);
        return (exit_code);
    }
	free_source(paths, slash, path, args);
	return (127); // Command not found
}

void	execute_command(t_ast_node *ast_node, t_shell *shell, char **envp)
{
	t_fds		*fd_red;
	char		*expanded_cmd;
	char		*expanded_arg;
	int			i;
	int			exit_code;
	int 		is_env_var;
	t_ast_node	*ast_orig;

	printf("DEBUG: execute_command called\n");
	i = -1;
	expanded_cmd = NULL;
	ast_orig = ast_node;
	fd_red = set_fd();
	if (!ast_node)
	{
		printf("DEBUG: ast_node is NULL\n");
		reset_fd(fd_red);
		return ;
	}
	if (ast_node->type == NODE_REDIR)
	{
		// Here resolve the redirection, you will only switch the output for the one you want
		if (redirection(ast_node, fd_red) == -1)
		{
			reset_fd(fd_red);
			shell->last_exit_code = 1;
			return;
		}
		while (ast_node->type == NODE_REDIR && ast_node)
		{
			ast_node = ast_node->u_content.redir.child;
			shell->last_exit_code = 0;
		}
	}
	if (ast_node && ast_node->type == NODE_PIPE)
	{
		reset_fd(fd_red);
    	execute_pipe(ast_node, shell, envp);
    	return ;
	}
	if (ast_node->type == NODE_ASSIGNMENT)
	{
		printf("DEBUG: Processing NODE_ASSIGNMENT\n");
		printf("DEBUG: Setting %s = %s\n", 
			ast_node->u_content.assign.name, ast_node->u_content.assign.value);
			
		// Nastavit proměnnou v environment
		env_set(&shell->env, ast_node->u_content.assign.name, ast_node->u_content.assign.value);
		shell->last_exit_code = 0;
		printf("DEBUG: Assignment completed\n");
		reset_fd(fd_red);
		return ;
	}
	if (ast_node->type == NODE_COMMAND)
	{
		printf("DEBUG: Processing NODE_COMMAND\n");
		int cmd_is_env_var = (ast_node->u_content.cmd.cmd_token_type == TOKEN_ENV_VAR 
			|| ast_node->u_content.cmd.cmd_token_type == TOKEN_EXIT_CODE);
		printf("DEBUG: Original cmd: '%s', cmd_token_type: %d, is_env_var: %d\n", 
			ast_node->u_content.cmd.cmd, ast_node->u_content.cmd.cmd_token_type, cmd_is_env_var);

		expanded_cmd = expand_variables(ast_node->u_content.cmd.cmd, 
                shell->env, shell->last_exit_code, cmd_is_env_var);
		if (!expanded_cmd)
		{
			printf("DEBUG: Expansion failed\n");
			shell->last_exit_code = 1;  
			reset_fd(fd_red);
			return ;
		}
		if (ft_strlen(expanded_cmd) == 0)
		{
			printf("minishell: : command not found\n");
			shell->last_exit_code = 127;
			free(expanded_cmd);
			reset_fd(fd_red);
			return ;
		}
		while (++i < ast_node->u_content.cmd.arg_count)
        {
			t_token_type token_type = ast_node->u_content.cmd.arg_token_types[i];
    
			if (ast_node->u_content.cmd.arg_token_types[i] == TOKEN_SINGLE_QUOTED) 
				continue; // Single quoted args are not expanded)
			is_env_var = (token_type == TOKEN_ENV_VAR || token_type == TOKEN_EXIT_CODE);
            expanded_arg = expand_variables(ast_node->u_content.cmd.args[i],
                    shell->env, shell->last_exit_code, 
        			is_env_var);
            free(ast_node->u_content.cmd.args[i]);
            ast_node->u_content.cmd.args[i] = expanded_arg;
        }
		printf("DEBUG: Checking builtins for command: '%s'\n", expanded_cmd);
		if (ft_strcmp(expanded_cmd, "exit") == 0)
			builtin_exit(shell, fd_red, ast_orig);
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
			exit_code = search_command(expanded_cmd, ast_node, shell->env, envp);
    		shell->last_exit_code = exit_code;
			printf("DEBUG: External command finished with exit code: %d\n", exit_code);
		}
	}	
    reset_fd(fd_red);
	free(expanded_cmd);
	printf("DEBUG: execute_command finished\n");
	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lformank <lformank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:53:23 by lformank          #+#    #+#             */
/*   Updated: 2025/06/04 14:38:34 by lformank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection.h"

void	redirection(t_ast_node *root, t_shell *shell, char **envp)
{
    int saved_stdout;
    char *filename;
    t_ast_node *child_command;

    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0)
    {
        perror("Error saving stdout");
       return ;
    }
    filename = root->u_content.redir.redir->file_or_delimiter;
    handle_output_redirection(filename);
    if (!filename) 
    {
        perror("Error: No filename provided for redirection");
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        return ;
    }
    
    
    child_command = root->u_content.redir.child;
    if (!child_command || child_command->type != NODE_COMMAND)
    {
        perror("Error: No command to execute after redirection");
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        return ;
    }
    execute_command(child_command, shell, envp);
    
    // 4. Obnovit původní stdout
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}

int handle_output_redirection(char *filename)
{
    int fd;

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Error opening file for output redirection");
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("Error redirecting stdout");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
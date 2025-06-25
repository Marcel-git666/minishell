/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:59:17 by marcel            #+#    #+#             */
/*   Updated: 2025/06/25 23:00:10 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection.h"

static int	setup_input_redir(char *file, int *saved_stdin)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin < 0 || handle_input_redirection(file) < 0)
		return (-1);
	return (0);
}

static int	setup_output_redir(char *file, int *saved_stdout)
{
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout < 0 || handle_output_redirection(file) < 0)
		return (-1);
	return (0);
}

static int	setup_append_redir(char *file, int *saved_stdout)
{
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout < 0 || handle_append_redirection(file) < 0)
		return (-1);
	return (0);
}

static int	setup_heredoc_redir(char *delimiter, int *saved_stdin)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin < 0 || handle_heredoc(delimiter) < 0)
		return (-1);
	return (0);
}

int	setup_redirection(t_redirection *redir, int *saved_stdin,
				int *saved_stdout)
{
	if (redir->type == REDIR_IN)
		return (setup_input_redir(redir->file_or_delimiter, saved_stdin));
	else if (redir->type == REDIR_OUT)
		return (setup_output_redir(redir->file_or_delimiter, saved_stdout));
	else if (redir->type == REDIR_APPEND)
		return (setup_append_redir(redir->file_or_delimiter, saved_stdout));
	else if (redir->type == REDIR_HEREDOC)
		return (setup_heredoc_redir(redir->file_or_delimiter, saved_stdin));
	return (0);
}

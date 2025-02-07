/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:26:56 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/07 16:22:43 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

typedef enum e_token_type
{
	TOKEN_CMD,			// Command (ls, echo, etc.)
	TOKEN_ARG,			// Argument (-l, filename)
	TOKEN_PIPE,			// Pipe (|)
	TOKEN_REDIR_IN,		// Input Redirection (<)
	TOKEN_REDIR_OUT,	// Output Redirection (>)
	TOKEN_APPEND_OUT,	// Append (>>)
	TOKEN_HEREDOC,		// Here-document (<<)
	TOKEN_STRING,		// Quoted string ("hello world")
	TOKEN_ENV_VAR,		// Environment variable ($HOME)
	TOKEN_EXIT_CODE,	// Special variable ($?)
	TOKEN_FILE,			// File name for redirection
	TOKEN_EOF			// End of input (NULL)
}	t_token_type;

typedef struct	s_token
{
	t_token_type	type;	// Type of token (e.g., TOKEN_CMD, TOKEN_PIPE)
	char			*value;	// The actual token value (e.g., "ls", "-l", "|")
	struct s_token	*next;	// Pointer to the next token (linked list)
}	t_token;

# endif

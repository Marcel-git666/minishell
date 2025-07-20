/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:26:56 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 18:08:05 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <stddef.h>

// Forward declaration
struct					s_lexer;
typedef struct s_lexer	t_lexer;

typedef enum e_token_type
{
	TOKEN_CMD,			// Command (ls, echo, etc.)
	TOKEN_ARG,			// Argument (-l, filename)
	TOKEN_PIPE,			// Pipe (|)
	TOKEN_REDIR_IN,		// Input Redirection (<)
	TOKEN_REDIR_OUT,	// Output Redirection (>)
	TOKEN_APPEND_OUT,	// Append (>>)
	TOKEN_HEREDOC,		// Here-document (<<)
	TOKEN_DOUBLE_QUOTED, // Quoted expandable string ("hello world")
	TOKEN_SINGLE_QUOTED, // Single quoted non expandable string ('hello world')
	TOKEN_ENV_VAR,		// Environment variable ($HOME)
	TOKEN_EXIT_CODE,	// Special variable ($?)
	TOKEN_FILE,			// File name for redirection
	TOKEN_DELIMITER,	// Assign a unique token type
	TOKEN_ASSIGNMENT,	// Variable assignment (VAR=value)
	TOKEN_EOF,			// End of input (NULL)
}	t_token_type;

typedef struct s_token
{
	t_token_type	type; // Type of token (e.g., TOKEN_CMD, TOKEN_PIPE)
	char			*value; // The actual token value (e.g., "ls", "-l", "|")
	struct s_token	*next; // Pointer to the next token (linked list)
}		t_token;

// token.c
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new_token);
void		free_tokens(t_token *head);
int			is_redirection_token(t_token_type type);

// token_extraction.c
int			is_special_char(char c);
char		*extract_word(const char *input, size_t *index,
				int is_delimiter_expected);
t_token		*extract_operator(const char *input, size_t *index);
char		*extract_env_var(const char *input, size_t *index);
int			check_next_token(t_lexer *lexer, size_t next_pos);

// token_string_extraction.c
char		*extract_single_quoted_string(t_lexer *lexer);
char		*extract_double_quoted_string(t_lexer *lexer);

#endif

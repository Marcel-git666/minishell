/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:14:51 by mmravec           #+#    #+#             */
/*   Updated: 2025/08/01 00:07:31 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

// Forward declarations
struct						s_token;
typedef struct s_token		t_token;
typedef enum e_token_type	t_token_type;

typedef struct s_lexer
{
	t_token		*tokens;
	const char	*input;
	size_t		i;
	int			is_file_expected;
	int			is_delimiter_expected;
}		t_lexer;

// lexer.c
int			add_token_from_input(t_lexer *lexer, int *is_first_word);

// lexer_core.c
void		skip_whitespace(const char *input, size_t *i);
void		init_lexer(t_lexer *lexer, const char *input);
int			process_current_char(t_lexer *lexer, int *is_first_word);
t_token		*lexer(const char *input);

// lexer_utils.c
int			is_valid_var_name(const char *word, int len);
int			check_assignment(char *word);
int			is_word_char(char c, int is_delimiter_expected);
int			handle_assignment_error(t_lexer *lexer);

// lexer_process_redirections.c
int			process_redirections(t_lexer *lexer);

// lexer_special_tokens.c
int			handle_pipe_token(t_lexer *lexer, int *is_first_word);
int			handle_quote_token(t_lexer *lexer, int *is_first_word);
char		*extract_compound_token(t_lexer *lexer, size_t start_pos);

// lexer_env_tokens.c
int			handle_special_tokens(t_lexer *lexer, int *is_first_word);

#endif

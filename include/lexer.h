/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:14:51 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/03 18:49:41 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

// Forward declarations
struct s_token;
typedef struct s_token t_token;
typedef enum e_token_type t_token_type;

typedef struct s_lexer
{
	t_token		*tokens;
	const char	*input;
	size_t		i;
	int			is_file_expected;
	int			is_delimiter_expected;
}		t_lexer;

// lexer.c
void		skip_whitespace(const char *input, size_t *i);
t_token		*lexer(const char *input);

// lexer_process_redirections.c
int			process_redirections(t_lexer *lexer);

// lexer_special_tokens.c
int			handle_special_tokens(t_lexer *lexer, int *is_first_word);

#endif

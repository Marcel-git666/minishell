/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:14:51 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/10 17:53:09 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef struct s_lexer
{
	t_token		*tokens;
	const char	*input;
	size_t		i;
	int			is_file_expected;
	int			is_delimiter_expected;
}		t_lexer;

#endif

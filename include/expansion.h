/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:45:37 by marcel            #+#    #+#             */
/*   Updated: 2025/08/01 16:32:28 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H
# include "env.h"

/*
 * Structure for maintaining state during expansion
 * Tracks input position, output position and quote states
 */
typedef struct s_expansion_state
{
	char	*input;
	char	*result;
	int		i;
	int		j;
	int		in_single_quotes;
	int		in_double_quotes;
	t_env	*env;
	int		exit_status;
}	t_expansion_state;

/*
 * Structure for size calculation state
 */
typedef struct s_size_calc
{
	char		*input;
	t_env		*env;
	int			exit_status;
	int			i;
	int			in_single_quotes;
	size_t		total_size;
}	t_size_calc;

/* Main expansion function */
char	*expand_variables(char *input, t_env *env, int exit_status,
			int is_env_var);
/* expansion_quotes.c */
void	process_single_quote(t_expansion_state *state);
void	process_double_quote(t_expansion_state *state);

/* Helper functions from expansion_utils.c */
char	*find_next_dollar(char *str);
int		parse_var_name(char *str, char **var_name);
char	*get_variable_value(char *var_name, t_env *env, int exit_status);
char	*expand_tilde(const char *path, t_env *env);

/* expansion_size.c */
size_t	calculate_expansion_size(char *input, t_env *env, int exit_status);

#endif
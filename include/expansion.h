/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:45:37 by marcel            #+#    #+#             */
/*   Updated: 2025/07/20 18:02:32 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H
# include "env.h"

/* Main expansion function */
char	*expand_variables(char *input, t_env *env, int exit_status,
			int is_env_var);

/* Helper functions from expansion_utils.c */
char	*find_next_dollar(char *str);
int		parse_var_name(char *str, char **var_name);
char	*get_variable_value(char *var_name, t_env *env, int exit_status);
char	*expand_tilde(const char *path, t_env *env);

#endif
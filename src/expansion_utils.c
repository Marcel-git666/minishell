/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:57:32 by marcel            #+#    #+#             */
/*   Updated: 2025/07/30 14:28:49 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

/*
 * Finds next dollar sign in string for variable expansion
 * Returns pointer to '$' character or NULL if none found
 */
char	*find_next_dollar(char *str)
{
	int	in_single_quotes;

	in_single_quotes = 0;
	while (*str)
	{
		if (*str == '\'' && !in_single_quotes)
			in_single_quotes = 1;
		else if (*str == '\'' && in_single_quotes)
			in_single_quotes = 0;
		else if (*str == '$' && !in_single_quotes)
			return (str);
		str++;
	}
	return (NULL);
}

/*
 * Parses braced variable name from ${VAR} syntax
 * Extracts variable name between braces and validates closing brace
 * Returns length including braces or 0 on error
 */
static int	parse_braced_var(char *str, char **var_name)
{
	int	len;
	int	i;

	len = 0;
	i = 1;
	while (str[i] && str[i] != '}')
	{
		len++;
		i++;
	}
	if (str[i] == '}')
	{
		*var_name = ft_substr(str, 1, len);
		return (len + 2);
	}
	return (0);
}

/*
 * Parses variable name from expansion syntax
 * Handles $?, ${VAR}, and $VAR formats
 * Returns length of parsed variable or 0 if invalid
 */
int	parse_var_name(char *str, char **var_name)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	if (str[0] == '?')
	{
		*var_name = ft_strdup("?");
		return (1);
	}
	if (str[0] == '{')
		return (parse_braced_var(str, var_name));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		len++;
		i++;
	}
	if (len > 0)
		*var_name = ft_substr(str, 0, len);
	return (len);
}

/*
 * Gets variable value from environment or special variables
 * Handles $? (exit status) and regular environment variables
 * Returns allocated string with value or empty string if not found
 */
char	*get_variable_value(char *var_name, t_env *env, int exit_status)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit_status));
	value = env_get(env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

/*
 * Expands tilde (~) to HOME directory path
 * Returns expanded path or original path if no tilde or HOME not set
 * Caller must free returned string
 */
char	*expand_tilde(const char *path, t_env *env)
{
	char	*home;
	char	*expanded;

	if (!path || path[0] != '~')
		return (ft_strdup(path));
	home = env_get(env, "HOME");
	if (!home)
		return (ft_strdup(path));
	if (path[1] == '\0')
		return (ft_strdup(home));
	if (path[1] == '/')
	{
		expanded = ft_strjoin(home, path + 1);
		return (expanded);
	}
	return (ft_strdup(path));
}

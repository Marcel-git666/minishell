/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:57:32 by marcel            #+#    #+#             */
/*   Updated: 2025/07/18 22:59:57 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

char	*find_next_dollar(char *str)
{
	while (*str)
	{
		if (*str == '$')
			return (str);
		str++;
	}
	return (NULL);
}

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

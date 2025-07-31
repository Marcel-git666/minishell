/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_compound.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:49:56 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/30 16:22:18 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

typedef struct s_compound_part
{
	char	*content;
	int		is_single_quoted;
	int		is_variable;
}	t_compound_part;

/*
 * Parses compound token into parts with metadata about quotes/variables
 * This preserves information about what was originally quoted
 */
static t_compound_part	*parse_compound_parts(char *compound, int *part_count)
{
	t_compound_part	*parts;
	int				pos;
	int				start;
	int				count;
	int				in_single_quotes;
	
	// First pass - count parts
	count = 0;
	pos = 0;
	in_single_quotes = 0;
	while (compound[pos])
	{
		if (compound[pos] == '\'' && !in_single_quotes)
		{
			in_single_quotes = 1;
			count++;
			pos++;
			while (compound[pos] && compound[pos] != '\'')
				pos++;
			if (compound[pos] == '\'')
			{
				pos++;
				in_single_quotes = 0;
			}
		}
		else if (compound[pos] == '"')
		{
			count++;
			pos++;
			while (compound[pos] && compound[pos] != '"')
				pos++;
			if (compound[pos] == '"')
				pos++;
		}
		else if (compound[pos] == '$')
		{
			count++;
			pos++;
			if (compound[pos] == '?')
				pos++;
			else if (compound[pos] == '{')
			{
				pos++;
				while (compound[pos] && compound[pos] != '}')
					pos++;
				if (compound[pos] == '}')
					pos++;
			}
			else
			{
				while (compound[pos] && (ft_isalnum(compound[pos]) || compound[pos] == '_'))
					pos++;
			}
		}
		else
		{
			count++;
			while (compound[pos] && compound[pos] != '\'' && compound[pos] != '"' && compound[pos] != '$')
				pos++;
		}
	}
	
	*part_count = count;
	if (count == 0)
		return (NULL);
		
	parts = malloc(sizeof(t_compound_part) * count);
	if (!parts)
		return (NULL);
	
	// Second pass - extract parts
	count = 0;
	pos = 0;
	
	while (compound[pos])
	{
		start = pos;
		
		if (compound[pos] == '\'')
		{
			// Single quoted part - no expansion, extract content without quotes
			pos++; // Skip opening quote
			start = pos;
			while (compound[pos] && compound[pos] != '\'')
				pos++;
			
			parts[count].content = ft_strndup(compound + start, pos - start);
			parts[count].is_single_quoted = 1;
			parts[count].is_variable = 0;
			
			if (compound[pos] == '\'')
				pos++; // Skip closing quote
			count++;
		}
		else if (compound[pos] == '"')
		{
			// Double quoted part - expansion allowed
			pos++; // Skip opening quote
			start = pos;
			while (compound[pos] && compound[pos] != '"')
				pos++;
			
			parts[count].content = ft_strndup(compound + start, pos - start);
			parts[count].is_single_quoted = 0;
			parts[count].is_variable = 0;
			
			if (compound[pos] == '"')
				pos++; // Skip closing quote
			count++;
		}
		else if (compound[pos] == '$')
		{
			// Variable part
			start = pos;
			pos++;
			if (compound[pos] == '?')
				pos++;
			else if (compound[pos] == '{')
			{
				pos++;
				while (compound[pos] && compound[pos] != '}')
					pos++;
				if (compound[pos] == '}')
					pos++;
			}
			else
			{
				while (compound[pos] && (ft_isalnum(compound[pos]) || compound[pos] == '_'))
					pos++;
			}
			
			parts[count].content = ft_strndup(compound + start, pos - start);
			parts[count].is_single_quoted = 0;
			parts[count].is_variable = 1;
			count++;
		}
		else
		{
			// Regular text
			start = pos;
			while (compound[pos] && compound[pos] != '\'' && compound[pos] != '"' && compound[pos] != '$')
				pos++;
			
			parts[count].content = ft_strndup(compound + start, pos - start);
			parts[count].is_single_quoted = 0;
			parts[count].is_variable = 0;
			count++;
		}
	}
	
	*part_count = count;
	return (parts);
}

/*
 * Expands a single variable and returns the expanded value
 */
static char	*expand_single_variable(char *var_str, t_env *env, int exit_status)
{
	char	*var_name;
	char	*result;
	int		var_len;

	if (ft_strlen(var_str) == 1) // Just "$" - invalid
		return (ft_strdup("$"));

	if (var_str[1] == '?')
		return (ft_itoa(exit_status));
		
	if (var_str[1] == '{')
	{
		// Handle ${VAR}
		var_len = ft_strlen(var_str) - 3; // Remove $, {, }
		if (var_len <= 0)
			return (ft_strdup(""));
		var_name = ft_substr(var_str, 2, var_len);
	}
	else
	{
		// Handle $VAR - check if valid variable name
		if (!ft_isalpha(var_str[1]) && var_str[1] != '_')
			return (ft_strdup("$")); // Invalid variable name, return literal $
		
		var_name = ft_substr(var_str, 1, ft_strlen(var_str) - 1);
	}
	
	result = env_get(env, var_name);
	free(var_name);
	
	if (result)
		return (ft_strdup(result));
	return (ft_strdup(""));
}

/*
 * Fixed compound token expansion that respects quote boundaries
 */
char	*expand_compound_token(char *compound, t_env *env, int exit_status)
{
	t_compound_part	*parts;
	char			*result;
	char			*temp;
	char			*expanded;
	int				part_count;
	int				i;

	if (!compound)
		return (NULL);
	parts = parse_compound_parts(compound, &part_count);
	if (!parts)
		return (ft_strdup(""));
	result = ft_strdup("");
	i = 0;
	while (i < part_count)
	{
		if (parts[i].is_single_quoted)
		{
			expanded = ft_strdup(parts[i].content);
		}
		else if (parts[i].is_variable)
		{
			expanded = expand_single_variable(parts[i].content, env, exit_status);
		}
		else
		{
			expanded = expand_variables(parts[i].content, env, exit_status, 0);
		}
		
		// Join with result
		temp = ft_strjoin(result, expanded);
		free(result);
		free(expanded);
		result = temp;
		
		i++;
	}
	
	// Cleanup
	i = 0;
	while (i < part_count)
	{
		free(parts[i].content);
		i++;
	}
	free(parts);
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:46:01 by marcel            #+#    #+#             */
/*   Updated: 2025/06/14 17:38:34 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "env.h"

/**
 * Najde další výskyt $ v stringu a vrátí jeho pozici
 */
static char *find_next_dollar(char *str)
{
    while (*str)
    {
        if (*str == '$')
            return (str);
        str++;
    }
    return (NULL);
}

/**
 * Parsuje název proměnné po $
 * Vrací délku názvu proměnné
 */
static int parse_var_name(char *str, char **var_name)
{
    int len;
    int i;
    
    len = 0;
    i = 0;
    
    // Speciální případ: $?
    if (str[0] == '?')
    {
        *var_name = ft_strdup("?");
        return (1);
    }
    
    // Speciální případ: ${VAR}
    if (str[0] == '{')
    {
        i = 1;
        while (str[i] && str[i] != '}')
        {
            len++;
            i++;
        }
        if (str[i] == '}')
        {
            *var_name = ft_substr(str, 1, len);
            return (len + 2); // +2 pro { a }
        }
        return (0); // Chyba - nezavřené {}
    }
    
    // Normální případ: $VAR
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
    {
        len++;
        i++;
    }
    
    if (len > 0)
        *var_name = ft_substr(str, 0, len);
    
    return (len);
}

/**
 * Získá hodnotu proměnné z environment
 */
static char *get_variable_value(char *var_name, t_env *env, int exit_status)
{
    char *value;
    
    // Speciální proměnná $?
    if (ft_strcmp(var_name, "?") == 0)
        return (ft_itoa(exit_status));
    
    // Hledej v environment
    value = env_get(env, var_name);
    if (value)
        return (ft_strdup(value));
    
    // Proměnná neexistuje - vrať prázdný string
    return (ft_strdup(""));
}

/**
 * Hlavní funkce pro variable expansion
 * Nahradí všechny $VAR a ${VAR} v input stringu
 */

char *expand_variables(char *input, t_env *env, int exit_status, int is_env_var)
{
    char *result;
    char *current;
    char *dollar_pos;
    char *var_name;
    char *var_value;
    char *temp;
    int var_len;
    size_t dollar_offset;
    
    printf("DEBUG: Expanding input: '%s'\n", input);
    if (!input)
        return (NULL);
    if (is_env_var)
        return (get_variable_value(input, env, exit_status));
    result = ft_strdup(input);
    current = result;
    
    while ((dollar_pos = find_next_dollar(current)) != NULL)
    {
        // Přeskoč $ a parsuj název proměnné
        var_len = parse_var_name(dollar_pos + 1, &var_name);
        
        if (var_len == 0)
        {
            // Není to validní proměnná, přeskoč $
            current = dollar_pos + 1;
            continue;
        }
        
        dollar_offset = dollar_pos - result;
        
        // Získej hodnotu proměnné
        var_value = get_variable_value(var_name, env, exit_status);
        
        // Vytvoř nový string s nahrazenou proměnnou
        *dollar_pos = '\0'; // Ukončí string před $
        temp = ft_strjoin(result, var_value);
        
        // Přidej zbytek po proměnné
        char *rest = dollar_pos + 1 + var_len;
        char *final_result = ft_strjoin(temp, rest);
        
        // Uvolni paměť
        free(result);
        free(temp);
        free(var_name);
        free(var_value);
        
        result = final_result;
        
        // Pokračuj hledáním od pozice, kde skončila nahrazená hodnota
        current = result + dollar_offset + ft_strlen(var_value);
    }
    printf("DEBUG: Result: '%s'\n", result);
    return (result);
}

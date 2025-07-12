/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:23:08 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/12 21:34:19 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdlib.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	// int				order;
}	t_env;

// Forward declarations
struct s_shell;
typedef struct s_shell t_shell;
struct s_ast_node;
typedef struct s_ast_node t_ast_node;


// env.c
t_env	*env_init(char **envp);
void	env_free(t_env *env);

// env_operations.c
int		env_set(t_env **env, const char *key, const char *value);
char	*env_get(t_env *env, const char *key);
int		env_unset(t_env **env, const char *key);

// env_utils.c  
char	**env_to_array(t_env *env);
char	*path_resolve(const char *command, t_env *env);
int		ft_envsize(t_env *lst);

// env_print.c
void	env_print(t_shell *shell);
void	env_print_sorted(t_shell *shell);
void	sort_env_array(char **array, int count);

#endif

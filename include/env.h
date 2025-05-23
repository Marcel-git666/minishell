/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:23:08 by mmravec           #+#    #+#             */
/*   Updated: 2025/05/11 21:43:11 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// env.c
t_env	*env_init(char **envp);
void	env_free(t_env *env);
void	env_print(t_env *env);
int		env_set(t_env **env, const char *key, const char *value);
char	*env_get(t_env *env, const char *key);
int		env_unset(t_env **env, const char *key);
char	**env_to_array(t_env *env);
char	*path_resolve(const char *command, t_env *env);

#endif

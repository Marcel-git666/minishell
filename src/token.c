/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcel <marcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:26:18 by mmravec           #+#    #+#             */
/*   Updated: 2025/07/20 11:12:51 by marcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"

/*
 * Creates new token with specified type and value
 * Allocates memory and duplicates value string
 * Returns token pointer or NULL on allocation failure
 */
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

/*
 * Adds token to end of linked list
 * Creates new list if head is NULL, otherwise appends to end
 */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}

/*
 * Frees entire token linked list and all allocated values
 * Traverses list and frees each token's value and structure
 */
void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

/*
 * Checks if token type represents a redirection operator
 * Returns 1 for <, >, >>, << operators, 0 otherwise
 */
int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND_OUT
		|| type == TOKEN_HEREDOC);
}

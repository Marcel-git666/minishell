/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:36:21 by mmravec           #+#    #+#             */
/*   Updated: 2025/02/07 18:58:38 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token Type: %d, Value: %s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}

void	execute_command(t_token *tokens)
{
	if (!tokens)
		return ;

	if (tokens->type == TOKEN_CMD
		&& ft_strncmp(tokens->value, "exit", 5) == 0)
	{
		printf("Exiting minishell...\n");
		free_tokens(tokens);
		exit(0);
	}
}

int	main(void)
{
	char	*input;
	t_token	*tokens;

	printf("Welcome to mini shell!\n");
	while (1)
	{
		input = readline("$ ");
		if (!input)
			break ;
		if (*input) // Add non-empty inputs to history
			add_history(input);
		tokens = lexer(input);
		print_tokens(tokens);
		execute_command(tokens);
		free_tokens(tokens);
		free(input);
	}
	return (0);
}

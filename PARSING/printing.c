/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:59:23 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/14 13:52:57 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_tokens_helper(t_token *tokens)
{
	if (!tokens)
		return ;
	if (tokens->type & TOKEN_ARG)
		printf("Token Type: %s, Value: %s\n", "Arg", tokens->value);
	else if (tokens->type & TOKEN_BUILT_IN)
		printf("Token Type: %s, Value: %s\n", "Built-in", tokens->value);
	else if (tokens->type & TOKEN_PIPE)
		printf("Token Type: %s, Value: %s\n", "pipe", tokens->value);
	else if (tokens->type & TOKEN_REDIRECT_IN)
		printf("Token Type: %s, Value: %s\n", "redirect_in", tokens->value);
	else if (tokens->type & TOKEN_REDIRECT_OUT)
		printf("Token Type: %s, Value: %s\n", "redirect_out", tokens->value);
	else if (tokens->type & TOKEN_APPEND)
		printf("Token Type: %s, Value: %s\n", "append", tokens->value);
	else if (tokens->type & TOKEN_HEREDOC)
		printf("Token Type: %s, Value: %s\n", "heredoc", tokens->value);
	else if (tokens->type & TOKEN_VAR)
		printf("Token Type: %s, Value: %s\n", "VAR", tokens->value);
	else if (tokens->type & TOKEN_SPACES)
		printf("Token Type: %s, Value: %s\n", "SPACE", tokens->value);
	else if (tokens->type & TOKEN_FILE)
		printf("Token Type: %s, Value: %s\n", "FILE", tokens->value);
	else
		printf("Token Type: UN (%d), Value: %s\n", tokens->type, tokens->value);
	tokens = tokens->next;
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	printf("\n---- Token List ----\n");
	current = tokens;
	while (current)
	{
		print_tokens_helper(current);
		current = current->next;
	}
	printf("--------------------\n");
}

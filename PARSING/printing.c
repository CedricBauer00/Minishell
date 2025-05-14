/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:59:23 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/14 10:42:13 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_tokens_helper(t_token *tokens)
{
	if (!tokens)
		return ;
	if (tokens->type & TOKEN_ARG)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "Arg", tokens->value);
	else if (tokens->type & TOKEN_BUILT_IN)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "Built-in", tokens->value);
	else if (tokens->type & TOKEN_PIPE)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "pipe", tokens->value);
	else if (tokens->type & TOKEN_REDIRECT_IN)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "redirect_in", tokens->value);
	else if (tokens->type & TOKEN_REDIRECT_OUT)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "redirect_out", tokens->value);
	else if (tokens->type & TOKEN_APPEND)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "append", tokens->value);
	else if (tokens->type & TOKEN_HEREDOC)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "heredoc", tokens->value);
	else if (tokens->type & TOKEN_VAR)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "VAR", tokens->value);
	else if (tokens->type & TOKEN_SPACES)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "SPACE", tokens->value);
	else if (tokens->type & TOKEN_FILE)
		printf(RED"Token Type: %s, Value: %s\n"DEFAULT, "FILE", tokens->value);
	else
		printf(RED"Token Type: UN (%d), Value: %s\n"DEFAULT, tokens->type, tokens->value);
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

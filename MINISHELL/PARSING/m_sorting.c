/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_sorting.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:21:35 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/22 16:40:43 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_for_command(t_token *tokens)
{
	if (is_built_in(tokens->value) == 1)
		return (tokens->type = TOKEN_BUILT_IN, 1);
	return (tokens->type = TOKEN_ARG, 1);
}

void	lex_tokens_correctly(t_token *tokens)
{
	int	cmd;

	cmd = 0;
	while (tokens)
	{
		if (tokens->next != NULL && tokens->type & (TOKEN_REDIRECT_IN
				| TOKEN_REDIRECT_OUT | TOKEN_APPEND)
			&& tokens->next->type & (TOKEN_ARG | TOKEN_BUILT_IN | TOKEN_VAR))
			tokens->next->type = TOKEN_FILE;
		else if (cmd == 0 && (tokens->type == TOKEN_ARG
				|| tokens->type == TOKEN_BUILT_IN || tokens->type == TOKEN_VAR))
			cmd = check_for_command(tokens);
		else if (cmd == 1 && (tokens->type == TOKEN_ARG
				|| tokens->type == TOKEN_BUILT_IN || tokens->type == TOKEN_VAR))
			tokens->type = TOKEN_ARG;
		else if (tokens->type == TOKEN_PIPE)
			cmd = 0;
		tokens = tokens->next;
	}
}

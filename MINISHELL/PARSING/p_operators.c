/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_operators.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:25:15 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/14 11:02:22 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	operator(t_main *main, int i, char c, t_gc_list *gc_list)
{
	if (c == '<' && i == 1)
		main->error = create_token(&main->tokens, TOKEN_REDIRECT_IN, "<", gc_list);
	else if (c == '>' && i == 1)
		main->error = create_token(&main->tokens, TOKEN_REDIRECT_OUT, ">", gc_list);
	else if (c == '|' && i == 1)
		main->error = create_token(&main->tokens, TOKEN_PIPE, "|", gc_list);
	else if (i == 2 && c == '>')
		main->error = create_token(&main->tokens, TOKEN_APPEND, ">>", gc_list);
	// else if (i == 2 && c == '<')
	// 	main->error = create_token(&main->tokens, TOKEN_HEREDOC, "<<", gc_list);
	return (i);
}

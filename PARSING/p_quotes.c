/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_quotes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:35:34 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/02 15:07:19 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	quotes_helper(t_main *main, int *i, int ws, t_gc_list **gc_list)
{
	main->word = gc_strndup(main->line + ws, *i - ws, gc_list);
	if (!main->word)
		return (-1);
	if (is_built_in(main->word) == 1)
		main->error = create_token(&main->tokens, TOKEN_BUILT_IN, \
			main->word, gc_list);
	else
		main->error = create_token(&main->tokens, TOKEN_ARG, \
			main->word, gc_list);
	if (main->error < 0)
		return (-1);
	(*i)++;
	return (0);
}

int	quotes(t_main *main, int *i, t_gc_list **gc_list)
{
	int	ws;

	ws = *i + 1;
	(*i)++;
	while (main->line[*i] && main->line[*i] != '\'')
		(*i)++;
	if (quotes_helper(main, i, ws, gc_list) < 0)
		return (-1);
	return (0);
}

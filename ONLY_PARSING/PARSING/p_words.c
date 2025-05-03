/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_words.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:23:13 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/01 16:15:21 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	words(t_main *main, int *i, int ws, t_gc_list **gc_list)
{
	int			len;
	t_gc_list	*find;

	ws = *i;
	while (main->line[*i] && (valid_char(main->line[*i], 1) == 0
			|| main->line[*i] == '_'))
		(*i)++;
	len = *i - ws;
	main->word = gc_strndup(main->line + ws, len, gc_list);
	if (is_built_in(main->word) == 1)
		main->error = create_token(&main->tokens, TOKEN_BUILT_IN, \
			main->word, gc_list);
	else
		main->error = create_token(&main->tokens, TOKEN_ARG, \
			main->word, gc_list);
	find = find_node(*gc_list, main->word);
	(void)find;
	delete_node(gc_list, find);
}

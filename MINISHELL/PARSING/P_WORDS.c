/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_WORDS.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:23:13 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/01 11:32:51 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	words(t_main *main, int i, int ws, t_gc_list *gc_list, int len)
{
	ws = i;
	while (main->line[i] && (ft_isalnum(main->line[i]) || main->line[i] =='_'))
		i++;
	len = i - ws;
	main->word = gc_strndup(main->line + ws, len, gc_list);
	if (is_built_in(main) == 1)
		main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
	else
		main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
	t_gc_list *find = find_node(gc_list, main->word);
	(void)find;
	delete_node(&gc_list, find);
}

// else if (ft_isalnum(main->line[i]) || main->line[i] == '_')
// 			{
// 				ws = i;
// 				// printf(GREEN"ws : %d\n"DEFAULT, ws);
// 				while (main->line[i] && (ft_isalnum(main->line[i]) || main->line[i] =='_'))
// 					i++;
// 				// printf(GREEN"valid_char() i : %d\n"DEFAULT, i);
// 				len = i - ws;
// 				// printf(RED"len %d\n"DEFAULT, len);
// 				// printf(RED"main->word :%p, main->line :%p\n"DEFAULT, main->word, main->line);
// 				main->word = gc_strndup(main->line + ws, len, gc_list);
// 				// printf(GREEN"main->word %p\n"DEFAULT, main->word);
// 				if (is_built_in(&main) == 1)
// 					main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
// 				else
// 				{
// 					// printf(GREEN"word = %s\n"DEFAULT, main->word);
// 					main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
// 				}
// 				t_gc_list *find = find_node(gc_list, main->word);
// 				(void)find;
// 				delete_node(&gc_list, find);
// 				// free(main->word);
// 				//i--;
// 			}
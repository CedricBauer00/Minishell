/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_quotes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:35:34 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/18 12:59:32 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	quotes_helper(t_main *main, int *i, int ws, t_gc_list *gc_list)
{
	main->word = gc_strndup(main->line + ws, *i - ws, gc_list);
	if (!main->word)
		return (-1);
	if (is_built_in(main->word) == 1)
		main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
	else
		main->error = create_token(&main->tokens, TOKEN_ARG, main->word, gc_list);
	if (main->error < 0)
		return (-1);
	(*i)++;
	return (0);
}

int quotes(t_main *main, int *i, t_gc_list *gc_list)
{
	int ws;

	ws = *i + 1;
	(*i)++;
	while (main->line[*i] && main->line[*i] != '\'')
		(*i)++;
	if (quotes_helper(main, i, ws, gc_list) < 0)
		return (-1);
	return 0;
}

// ws = i + 1;
// i++;
// while (1)
// {
// 	while (main.line[i] && main.line[i] != '\'') //0x5
// 		i++;
// 	if (main.line[i] == '\'')
// 	{
// 		main.word = gc_strndup(main.line + ws, i - ws, gc_list);  //0x0001 + >> main.word = 0x0002 . 0x00005 + ws
// 		if (!main.word)
// 			return (perror("ERROR\nAllocating main.word failed!\n"), all_free(&gc_list), -1);
// 		if (is_built_in(&main) == 1)
// 			main.error = create_token(&main.tokens, TOKEN_BUILT_IN, main.word, gc_list);
// 		else
// 			main.error = create_token(&main.tokens, TOKEN_WORD, main.word, gc_list);
// 		if (main.error < 0)
// 			return (perror("ERROR\nToken creation failed!\n"), all_free(&gc_list), -1);
// 		i++;
// 		t_gc_list *todelte = find_node(gc_list, main.word);
// 		// printf(RED"main.word : %p\n"DEFAULT, main.word);
// 		delete_node(&gc_list, todelte);
// 		break ;
// 		printf(RED"enter\n"DEFAULT);
// 	}
// 	main.next_line = readline("> ");
// 	if (!main.next_line)
// 		return (perror("ERROR\nFailed!\n"), all_free(&gc_list), -1);
// 	printf(YELLOW"before usig strjoin%p\n"DEFAULT, main.line);
// 	main.line = gc_strjoin(main.line, main.next_line, gc_list);  //0x0001 ,0x0002 >> main.line 0x0005
// 	printf("main.line : %p\n", main.line);
// 	//free(main.line);
// 	printf(YELLOW"after usig strjoin%p\n"DEFAULT, main.line);
// 	if (main.next_line)
// 	{
// 		printf(BLUE"free next_line\n"DEFAULT);
// 		free(main.next_line);
// 		main.next_line = NULL;
// 	}
// }
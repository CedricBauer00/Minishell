/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_words.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:23:13 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/06 16:46:28 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	words(t_main *main, int *i, int ws, t_gc_list *gc_list)
{
	int	len;
	
	ws = *i;
	while (main->line[*i] && (valid_char(main->line[*i]) == 1 || main->line[*i] == '_'))
		(*i)++;
	len = *i - ws;
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
// {
// ws = i;
// // printf(GREEN"ws : %d\n"DEFAULT, ws);
// while (main->line[i] && (ft_isalnum(main->line[i]) || main->line[i] =='_'))
// i++;
// // printf(GREEN"valid_char() i : %d\n"DEFAULT, i);
// len = i - ws;
// // printf(RED"len %d\n"DEFAULT, len);
// // printf(RED"main->word :%p, main->line :%p\n"DEFAULT, main->word, main->line);
// main->word = gc_strndup(main->line + ws, len, gc_list);
// // printf(GREEN"main->word %p\n"DEFAULT, main->word);
// if (is_built_in(&main) == 1)
// main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
// else
// {
// // printf(GREEN"word = %s\n"DEFAULT, main->word);
// main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
// }
// t_gc_list *find = find_node(gc_list, main->word);
// (void)find;
// delete_node(&gc_list, find);
// // free(main->word);
// //i--;
// }	



// int	check_operator2(t_main *main, int *i, t_gc_list *gc_list)
// {
// 	if (main->line[*i] == '"')
// 	{
// 		if (dquotes(main,i, gc_list) < 0)
// 			return (fprintf(stderr, BLUE"ERROR\nQuotes failed!\n"DEFAULT), all_free(&gc_list), -1);
// 	}
// 	else if (main->line[*i] && main->line[*i + 1] && main->line[*i + 2] &&
// 		ft_strncmp(main->line + *i, "EOF", 3) == 0 && !ft_isalnum(main->line[*i + 3]))
// 	{
// 		main->error = create_token(&main->tokens, TOKEN_EOF, "EOF", gc_list);
// 		*i += 2;
// 	}
// 	else if (ft_isalnum(main->line[*i]) || main->line[*i] == '_' || main->line[*i] == '-')
// 		words(main, i, 0, gc_list);
// 	else if (main->line[*i] == '$')
// 	{
// 		if (expands(main, i, 0, gc_list) < 0)
// 			return (perror("ERROR\nExpand failed!\n"), -1);
// 	}
// 	else
// 		printf("Warning: Unrecognized character '%c' at position %d\n", main->line[*i], *i);
// 	return (0);
// }

// int	check_operator(t_main *main, int *i, t_gc_list *gc_list)
// {
// 	if (main->line[*i] == '>' && main->line[*i + 1] == '>')
// 	{
// 		write(1, "here!\n", 6);
// 		*i += operator(main, 2, main->line[*i], gc_list);
// 		if (i < 0)
// 			return (perror("ERROR\nsyntax main->error near unexpected token `>`"), all_free(&gc_list), -1);
// 	}
// 	else if (main->line[*i] == '<' && main->line[*i + 1] == '<') //fix
// 	{
// 		*i = heredoc(main, *i, gc_list);
// 		if (i < 0)
// 			return(perror("ERROR\nHeredoc failed!\n"), all_free(&gc_list), -1);
// 	}
// 	else if (main->line[*i] == '<' || main->line[*i] == '>' || main->line[*i] == '|')
// 		*i += operator(main, 1, main->line[*i], gc_list);
// 	else if (main->line[*i] == '\'')
// 	{	
// 		if (quotes(main, i, gc_list) < 0)
// 			return (printf("ERROR\nQuotes failed!\n"), all_free(&gc_list), -1);
// 	}
// 	else
// 	{
// 		if (check_operator2(main, i, gc_list) < 0)
// 			return (-1);
// 	}
// 	return (0);
// }
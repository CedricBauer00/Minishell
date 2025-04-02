/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_DQUOTES.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 09:42:44 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/01 16:47:57 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int dquotes(t_main *main, int *i, t_gc_list *gc_list)
{
	int ws;

    ws = *i + 1; // Start after the opening quote
    (*i)++;      // Move past the opening quote
    while (main->line[*i]) // Loop through the input line
    {
        while (main->line[*i] && main->line[*i] != '"') // Find closing quote
        {
            (*i)++;
        }
        if (main->line[*i] == '"') // If closing quote is found
        {
            // Extract the word between quotes
            main->word = gc_strndup(main->line + ws, *i - ws, gc_list);
            if (!main->word)
                return (-1);
            // Create token for the extracted word
            if (is_built_in(main) == 1)
                main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
            else
                main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
            if (main->error < 0)
                return (-1);
            (*i)++; // Move past the closing quote
            return 0; // Successfully processed one quoted segment
        }
		//else
		//error message
    }

    return 0; // Return success if no errors occurred
}

// ws = i + 1;
// i++;
// while (1)
// {
// 	while (main->line[i] && main->line[i] != '"')
// 		i++;
// 	if (main->line[i] == '"')
// 	{
// 		main->word = gc_strndup(main->line + ws, i - ws, gc_list);
// 		if (!main->word)
// 		{
// 			return (perror("ERROR\nAllocating main->word failed!\n"), all_free(&gc_list), -1);
// 			exit(1);
// 		}
// 		if (is_built_in(main) == 1)
// 			main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
// 		else
// 			main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
// 		if (main->error < 0)
// 		{
// 			return (perror("ERROR\nToken creation failed!\n"), all_free(&gc_list), -1);
// 			exit(1);
// 		}
// 		i++;
// 		t_gc_list *todelte = find_node(gc_list, main->word);
// 		// printf(RED"main->word : %p\n"DEFAULT, main->word);
// 		delete_node(&gc_list, todelte);
// 		break ;
// 		// printf(RED"enter\n"DEFAULT);
// 	}
// 	main->next_line = readline("> ");
// 	if (!main->next_line)
// 		return (perror("ERROR\nFailed!\n"), all_free(&gc_list), -1);
	
// 	main->new = gc_strjoin(main->line, main->next_line, gc_list);
// 	if (!main->new)
// 	{
// 		free(main->next_line);
// 		return (perror("ERROR\nAllocating main->new failed!\n"), all_free(&gc_list), -1);
// 	}
// 	main->line = gc_strjoin(main->line, main->next_line, gc_list);
// 	free(main->next_line);
// 	main->next_line = NULL;
// }
// return (0);
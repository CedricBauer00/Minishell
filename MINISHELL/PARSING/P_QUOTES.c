/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_QUOTES.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:35:34 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/01 15:58:44 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int quotes(t_main *main, int *i, t_gc_list *gc_list)
{
    int ws;

    ws = *i + 1; // Start after the opening quote
    (*i)++;      // Move past the opening quote

    while (main->line[*i]) // Loop through the input line
    {
        while (main->line[*i] && main->line[*i] != '\'') // Find closing quote
        {
            (*i)++;
        }
        if (main->line[*i] == '\'') // If closing quote is found
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
        // else
        // {
		// 	main->next_line = readline("> ");
		// 	if (!main->next_line)
		// 		return (-1);
		// 	main->line = gc_strjoin(main->line, main->next_line, gc_list);
		// 	if (main->next_line)
		// 	{
		// 		free(main->next_line);
		// 		main->next_line = NULL;
		// 	}
        // }
    }

    return 0; // Return success if no errors occurred
}


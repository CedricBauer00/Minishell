/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:54:17 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/20 12:51:13 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	heredoc(t_main *main, int i, t_gc_list *gc_list)
{
	int		j;
	int		end;
	char	*word;

	if (main->line[i + 2] == '\0')
	{
		main->error = create_token(&main->tokens, TOKEN_HEREDOC, NULL, gc_list);
		return (i + 2);
	}
	j = i + 2;
	while (main->line[j] && check_char(main, j, 0) == 1)
		j++;
	end = is_quote(main, j);
	word = gc_strndup(&main->line[j], end - j, gc_list);
	if (!word)
		return (-1);
	main->error = create_token(&main->tokens, TOKEN_HEREDOC, word, gc_list);
	return (end);
}

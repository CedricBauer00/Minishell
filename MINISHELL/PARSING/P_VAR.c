/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_VAR.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:35:00 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/01 11:36:41 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	variables(t_main *main, int i, int ws, int len, t_gc_list *gc_list)
{
	ws = i++;
	while (main->line[i] && (valid_char(main->line[i]) || main->line[i] == '_'))
		i++;
	len = i - ws;
	if (len > 1)
		main->word = gc_strndup(main->line + ws, len, gc_list);
	else
		main->word = ft_strdup("$");
	main->error = create_token(&main->tokens, TOKEN_VAR, main->word, gc_list);
	free(main->word);
}

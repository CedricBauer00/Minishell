/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_HEREDOC->c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:54:17 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/04 11:56:48 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	heredoc(t_main *main, int i, t_gc_list *gc_list)
{
	int	j;
	int	ws;
	char	*str;

	// if (main->line[i + 2] == '\0')
	// {
	// 	write(1, "heredoc\n", 8);
	// 	return (printf("ERROR\nHeredoc missing delimiter!\n"), -1);
	// }
	printf("i = %d\n", i+ 2);
	printf("i = %c\n", main->line[i + 2]);
	j = i + 2;
	// printf("j1 = %c\n", main->line[j]);
	// printf("j1 = %d\n", j);

	while (main->line[j] && check_char(main, j, 0) == 0)
		j++;
	ws = j;
	j = is_quote(main, j);
	str = gc_strndup(&main->line[j], ws - j, gc_list);

	// printf("j2 = %c\n", main->line[j]);
	// printf("j2 = %d\n", j);
	while (main->line[j] && !ft_isspace(main->line[j]))
		j++;
	// printf("j3 = %c\n", main->line[j]);
	// printf("j3 = %d\n", j);
	if (main->line[i + 2] != '\0')
		main->error = create_token(&main->tokens, TOKEN_HEREDOC, "<<", gc_list);
	return (i += 2);
}
z
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

	//|| ft_isspace(main->line[i + 2])
	if (!main->line[i + 2])
		return (printf("ERROR\nHeredoc missing delimiter!\n")-1);
	j = i + 2;
	printf("j1 = %c\n", main->line[j]);
	printf("j1 = %d\n", j);

	while (check_char(main, j, 0) == 0)
		j++;
	is_quote(main, j);
	printf("j2 = %c\n", main->line[j]);
	printf("j2 = %d\n", j);
	ws = j;
	while (!ft_isspace(main->line[j]))
		j++;
	printf("j3 = %c\n", main->line[j]);
	printf("j3 = %d\n", j);
	main->error = create_token(&main->tokens, TOKEN_HEREDOC, "<<", gc_list);
	write(1, "here3\n", 6);
	

	return (i += 2);
}

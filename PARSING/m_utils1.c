/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_utils1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:53:21 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/13 10:10:08 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	set_default(t_main *main)
{
	main->tokens = NULL;
	main->line = NULL;
	main->last_status_exit = 0;
	main->next_line = NULL;
	main->new = NULL;
	main->word = NULL;
	main->temp_for_line = NULL;
	main->error = 0;
	return ;
}

int	check_operator2(t_main *main, int *i, t_gc_list **gc_list)
{
	if (main->line[*i] == '"')
	{
		if (dquotes(main, i, gc_list) < 0)
			return (printf("ERROR\nQuotes failed!\n"), -1); //ERROR? Even error message required?
	}
	else if (main->line[*i] == '$')
	{
		if (expands(main, i, 0, gc_list) < 0)
			return (perror("ERROR\nExpand failed!\n"), -1); //ERROR? Even error message required?
	}
	else if (!ft_isspace(main->line[*i]))
		words(main, i, 0, gc_list);
	else
		printf("Warning: Unrecognized character '%c' at position %d\n", \
			main->line[*i], *i); //ERROR? Even error message required?
	return (0);
}

int	check_operator(t_main *main, int *i, t_gc_list **gc_list)
{
	if (ft_isspace(main->line[*i]))
		main->error = create_token(&main->tokens, TOKEN_SPACES, " ", gc_list);
	while (main->line[*i] && ft_isspace(main->line[*i]))
		(*i)++;
	if (main->line[*i] == '\0')
		return (0);
	if (main->line[*i] == '>' && main->line[*i + 1] == '>')
		*i += operator(main, 2, main->line[*i], gc_list);
	else if (main->line[*i] == '<' && main->line[*i + 1] == '<')
		*i = heredoc(main, *i, gc_list);
	else if (main->line[*i] == '<' || main->line[*i] == '>'
		|| main->line[*i] == '|' || main->line[*i] == '&')
		*i += operator(main, 1, main->line[*i], gc_list);
	else if (main->line[*i] == '\'')
	{
		if (quotes(main, i, gc_list) < 0)
			return (printf("ERROR\nQuotes failed!\n"), -1); //ERROR? Even error message required?
	}
	else if (check_operator2(main, i, gc_list) < 0)
		return (-1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_dquotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 09:42:44 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/02 15:07:01 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	get_spaces(t_main *main, int *i, int k, t_gc_list **gc_list)
{
	int		j;
	char	*str;

	j = 0;
	k = 0;
	while (ft_isspace(main->line[*i]))
	{
		j++;
		(*i)++;
	}
	str = do_alloc(gc_list, j + 1, TYPE_SINGLE_PTR, "get_spaces");
	if (!str)
		return (-1);
	str[j] = '\0';
	while (++k < j)
		str[k] = ' ';
	main->error = create_token(&main->tokens, TOKEN_ARG, str, gc_list);
	return (0);
}

int	read_word(t_main *main, int *i, int k, t_gc_list **gc_list)
{
	int		j;
	int		m;
	char	*str;

	j = 0;
	m = *i;
	while (main->line[m] && main->line[m] != '"' && main->line[m] != '$')
	{
		j++;
		m++;
	}
	str = do_alloc(gc_list, j + 1, TYPE_SINGLE_PTR, "read_word");
	if (!str)
		return (-1);
	str[j] = '\0';
	while (++k < j)
	{
		str[k] = main->line[*i];
		(*i)++;
	}
	if (is_built_in(str) == 1)
		main->error = create_token(&main->tokens, TOKEN_BUILT_IN, str, gc_list);
	else
		main->error = create_token(&main->tokens, TOKEN_ARG, str, gc_list);
	return (0);
}

int	dquotes_helper(t_main *main, int *i, t_gc_list **gc_list)
{
	while (main->line[*i] && main->line[*i] != '"')
	{
		if (main->line[*i] == '$')
		{
			if (expands(main, i, 0, gc_list) < 0)
				return (perror("ERROR\nExpand failed!\n"), -1);
		}
		if (main->line[*i] && main->line[*i] != '"' && main->line[*i] != '$')
		{
			if (read_word(main, i, -1, gc_list) < 0)
				return (-1);
		}
	}
	return (0);
}

int	dquotes(t_main *main, int *i, t_gc_list **gc_list)
{
	(*i)++;
	if (dquotes_helper(main, i, gc_list) < 0)
		return (-1);
	if (main->line[*i] != '"')
		return (printf(RED"ERROR\nUnclosed quotes!\n"DEFAULT), 0);
	if (main->line[*i] != '\0')
		(*i)++;
	return (0);
}

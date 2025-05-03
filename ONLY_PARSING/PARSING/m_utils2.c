/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_utils2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:19:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/24 18:16:34 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_char(t_main *main, int i, int indic)
{
	if (indic == 0)
	{
		if (ft_isspace(main->line[i]))
			return (1);
	}
	else
	{
		if (ft_isspace(main->line[i]))
			return (1);
		if (main->line[i] == '|' || main->line[i] == '<'
			|| main->line[i] == '>' || main->line[i] == '$'
			|| main->line[i] == '"' || main->line[i] == '\'')
			return (1);
	}
	return (0);
}

int	check_quote(t_main *main)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	while (main->line[i])
	{
		if (j == 0 && (main->line[i] == '"' || main->line[i] == '\''))
		{
			c = main->line[i];
			j = 1;
		}
		else if (j == 1 && main->line[i] == c)
			j = 0;
		i++;
	}
	if (j == 1)
	{
		if (c == '"')
			return (choose_error_statement(4, "\""), -1);
		else
			return (choose_error_statement(4, "\'"), -1);
	}
	return (0);
}

int	is_quote(t_main *main, int i)
{
	int		j;
	char	c;

	j = 0;
	--i;
	while (main->line[++i] != '\0')
	{
		if (j == 1 && main->line[i] == c)
			j = 0;
		else if (j == 0 && (main->line[i] == '"' || main->line[i] == '\''))
		{
			c = main->line[i];
			j = 1;
		}
		else if (j == 0 && (main->line[i] == '|' || main->line[i] == '<'
				|| main->line[i] == '>' || valid_char(main->line[i], 0) == 1))
			break ;
	}
	return (i);
}

int	valid_char(int c, int indic)
{
	if (indic == 0)
		return (ft_isspace(c));
	else
	{
		if (c == '\'' || c == '"' || c == '|' || c == '<'
			|| c == '>' || c == '$')
			return (1);
		if (ft_isspace(c) == 1)
			return (1);
	}
	return (0);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\f' || c == '\v');
}

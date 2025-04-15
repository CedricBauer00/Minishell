/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:19:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/15 10:34:16 by cbauer           ###   ########.fr       */
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
		if (main->line[i] == '|' || main->line[i] == '<' || main->line[i] == '>'
			|| main->line[i] == '$' || main->line[i] == '"' || main->line[i] == '\'')
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
		return (printf(RED"ERROR\nUnclosed quotes!\n"DEFAULT), -1);
	return (0);
}

int is_quote(t_main *main, int i)
{
	int		j;
	char	c;
	
	j = 0;
	--i;
	while (main->line[++i] != '\0')
	{
		// printf("i = %d\n", i);
		if (j == 1 && main->line[i] == c)
			j = 0;
		else if (j == 0 && (main->line[i] == '"' || main->line[i] == '\''))
		{
			c = main->line[i];
			j = 1;
		}
		else if (j == 0 && (main->line[i] == '|' || main->line[i] == '$'
			|| main->line[i] == '<' || main->line[i] == '>' || valid_char(c, 0) == 1))
			break ;
	}
	return (i);
}

void	set_default(t_main *main)
{
	main->tokens = NULL;
	main->line = NULL;
	main->last_status_exit = 0;
	main->envp = NULL;
	main->next_line = NULL;
	main->new = NULL;
	main->word = NULL;
	main->temp_for_line = NULL;
	main->error = 0;
	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_EXPAND.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:58:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/02 11:36:20 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int is_valid_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

void	expands(t_main *main, int *i, t_gc_list *gc_list)
{
	int		ws;
	int		j;
	char	*var;
	int		len;
	char	*value;

	ws = *i;
	j = 0;
	value = NULL; //wenn value später immernoch NULL ist wurde sie nicht gefunden
	(*i)++;
	//edge case für nur $ input
	// if (main->line[*i] == '?' || main->line[*i] == '$') this case needs handling ????
	if (main->line[*i] == '?')
	{
		value = ft_itoa(main->last_status_exit);
		(*i)++;
	}
	else if (main->line[*i] == '$')
	{
		value = ft_itoa(getpid());
		(*i)++;
	}
	else
	{
		while (is_valid_char(main->line[*i]))
			(*i)++;
		var = gc_strndup(main->line + ws + 1, (*i) - ws - 1, gc_list);
		if (!var)
			return ;
		while (main->envp[j] != NULL)
		{
			len = ft_strlen(var);
			if (ft_strncmp(main->envp[j], var, len) == 0 && main->envp[j][len] == '=')
			{
				value = ft_strdup(main->envp[j] + len + 1);
				break ;
			}
			j++;
		}
		if (!value)
			value = ft_strdup("");
		create_token(&main->tokens, TOKEN_VAR, value, gc_list);
		if (value)
			free(value);
	}
	return ;
}

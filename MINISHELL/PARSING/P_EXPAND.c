/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_EXPAND.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:58:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/02 14:42:47 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int is_valid_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

int	expands(t_main *main, int *i, t_gc_list *gc_list)
{
	int		ws;
	int		j;
	char	*var;
	int		len;
	char	*value;

	ws = *i;
	j = 0;
	var = NULL;
	value = NULL; //wenn value später immernoch NULL ist wurde sie nicht gefunden
	(*i)++;
	//edge case für nur $ input
	// if (main->line[*i] == '?' || main->line[*i] == '$') this case needs handling ????
	if (main->line[*i] && main->line[*i] == '?')
	{
		value = ft_itoa(main->last_status_exit);
		if (!value)
			return (-1);
		(*i)++;
	}
	else if (main->line[*i] && main->line[*i] == '$')
	{
		value = ft_itoa(getpid());
		if (!value)
			return (-1);
		(*i)++;
	}
	else
	{
		while (main->line[*i] && is_valid_char(main->line[*i]))
			(*i)++;
		if ((*i) - ws - 1 <= 0)
		{
			create_token(&main->tokens, TOKEN_WORD, "$", gc_list);
			if (value)
				free(value);
			return (0);
		}
		var = gc_strndup(main->line + ws + 1, (*i) - ws - 1, gc_list);
		if (!var)
			return (-1);
		if (ft_strlen(var) == 0)
		{
			free(var);
			return (0);
		}
		while (main->envp[j])
		{
			len = ft_strlen(var);
			// ft_strlen(main->envp[j]) > len
			if (ft_strncmp(main->envp[j], var, len) == 0 \
				&& main->envp[j][len] == '=')
			{
				value = ft_strdup(main->envp[j] + len + 1);
				if (!value)
				{
					free(var);
					return (-1);
				}
				break ;
			}
			j++;
		}
		if (!value)
			value = ft_strdup("");
		if (value)
		{
			create_token(&main->tokens, TOKEN_VAR, value, gc_list);
			free(value);
		}
		if (var)
			free(var);
	}
	return (0);
}

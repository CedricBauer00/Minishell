/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_EXPAND.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:58:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/05 12:15:24 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int is_valid_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

int	expands(t_main *main, int *i, int ws, t_gc_list *gc_list) // $PATH=HELLo // multiple
{
	t_shell *shell = get_shell(gc_list);
	int		j;
	char	*var;
	int		len;
	char	*value;

	ws = *i;
	j = 0;
	var = NULL;
	value = NULL; //wenn value später immernoch NULL ist wurde sie nicht gefunden
	(*i)++;
	if (main->line[*i] && main->line[*i] == '?')
	{
		write(1, "here1\n", 6);
		value = ft_itoa(shell->last_status_exit);
		if (!value)
			return (-1);
		main->error = create_token(&main->tokens, TOKEN_WORD, value, gc_list);
		(*i)++;
		if (value)
			free(value);
	}
	else if (main->line[*i] && main->line[*i] == '$')
	{
		write(1, "here2\n", 6);
		value = ft_itoa(getpid());
		if (!value)
			return (-1);
		main->error = create_token(&main->tokens, TOKEN_WORD, value, gc_list);
		(*i)++;
		if (value)
			free(value);
	}
	else
	{
		while (main->line[*i] && is_valid_char(main->line[*i]))
			(*i)++;
		if ((*i) - ws - 1 <= 0)
		{
			write(1, "this is here!\n", 14);
			main->error = create_token(&main->tokens, TOKEN_WORD, "$", gc_list);
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
			write(1, "is var\n", 7);
			main->error = create_token(&main->tokens, TOKEN_VAR, value, gc_list);
			free(value);
		}
		if (var)
			free(var);
	}
	return (0);
}

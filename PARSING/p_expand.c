/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_expand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:58:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/16 16:18:52 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*check_for_var(t_main *main, char *var, int len, t_gc_list **gc_list)
{
	int		j;
	char	*value;
	t_shell	*shell;

	(void)main;
	j = 0;
	shell = get_shell();
	value = NULL;
	while (shell->my_envp[j])
	{
		len = ft_strlen(var);
		if (ft_strncmp(shell->my_envp[j], var, len) == 0 \
			&& shell->my_envp[j][len] == '=')
		{
			value = gc_strdup(shell->my_envp[j] + len + 1, gc_list);
			if (!value)
				return (NULL);
			break ;
		}
		j++;
	}
	return (value);
}

int	only_dollar_sign(t_main *main, t_gc_list **gc_list)
{
	main->error = create_token(&main->tokens, TOKEN_ARG, "$", gc_list);
	return (0);
}

int	expand_helper(t_main *main, int *i, int ws, t_gc_list **gc_list)
{
	char	*var;
	char	*value;
	int		len;

	len = 0;
	var = NULL;
	value = NULL;
	while (main->line[*i] && is_valid_char(main->line[*i]))
		(*i)++;
	if ((*i) - ws - 1 <= 0)
		return (only_dollar_sign(main, gc_list), 0);
	var = gc_strndup(main->line + ws + 1, (*i) - ws - 1, gc_list);
	if (!var)
		return (-1);
	if (ft_strlen(var) == 0)
		return (free(var), 0);
	value = check_for_var(main, var, len, gc_list);
	if (!value)
		value = gc_strdup("", gc_list);
	if (value)
		main->error = create_token(&main->tokens, TOKEN_VAR, value, gc_list);
	return (0);
}

int	special_character(t_main *main, int *i, char *value, t_gc_list **gc_list)
{
	value = ft_itoa(get_shell()->last_status_exit);
	if (!value)
		return (-1);
	main->error = create_token(&main->tokens, TOKEN_ARG, value, gc_list);
	(*i)++;
	if (value)
		free(value);
	return (0);
}

int	expands(t_main *main, int *i, int ws, t_gc_list **gc_list)
{
	ws = *i;
	(*i)++;
	if (main->line[*i] && main->line[*i] == '?')
	{
		if (special_character(main, i, NULL, gc_list) < 0)
			return (-1);
	}
	else
	{
		if (expand_helper(main, i, ws, gc_list) < 0)
			return (-1);
	}
	return (0);
}

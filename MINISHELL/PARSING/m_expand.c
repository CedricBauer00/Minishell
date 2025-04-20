/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_expand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:58:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/16 16:32:58 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int is_valid_char(char c)
{
    return ((ft_isalnum(c) || c == '_') && c != '=' && c != '$');
}

char	*check_for_var(t_main *main, char *var, int len, t_gc_list *gc_list)
{
	int		j;
	char	*value;

	j = 0;
	value = NULL;
	while (main->envp[j])
	{
		len = ft_strlen(var);
		if (ft_strncmp(main->envp[j], var, len) == 0 \
			&& main->envp[j][len] == '=')
		{
			value = gc_strdup(main->envp[j] + len + 1, gc_list);
			if (!value)
			{
				free(var);
				return (NULL);
			}
			break ;
		}
		j++;
	}
	return (value);
}

int	only_$(t_main *main, t_gc_list *gc_list)
{
	main->error = create_token(&main->tokens, TOKEN_ARG, "$", gc_list);
	return (0);
}

int	expand_helper(t_main *main, int *i, int ws, t_gc_list *gc_list)
{
	char	*var = NULL;
	char	*value = NULL;
	int		len;
	int		j;
	
	j = 0;
	len = 0;
	while (main->line[*i] && is_valid_char(main->line[*i])) //möglicherweise wird hier error geben wenn man mit export $VARS declared die special chars haben 
		(*i)++;
	if ((*i) - ws - 1 <= 0)
		return (only_$(main, gc_list), 0);
	var = gc_strndup(main->line + ws + 1, (*i) - ws - 1, gc_list);
	if (!var)
		return (-1);
	printf("var: %s\n", var);
	if (ft_strlen(var) == 0)
	{
		free(var);
		return (0);
	}
	value = check_for_var(main, var, len, gc_list);
	if (!value)
		value = gc_strdup("", gc_list);
	if (value)
		main->error = create_token(&main->tokens, TOKEN_VAR, value, gc_list);
	return (0);
}

int	special_character(t_main *main, int *i, char *value, t_gc_list *gc_list)
{
	t_shell *shell = get_shell(gc_list);

	value = ft_itoa(shell->last_status_exit);
	if (!value)
		return (-1);
	main->error = create_token(&main->tokens, TOKEN_ARG, value, gc_list);
	(*i)++;
	if (value)
		free(value);
	return (0);
}

int	expands(t_main *main, int *i, int ws, t_gc_list *gc_list) // $PATH=HELLo // expand ausführen wenn string in $VAR steht der vorher mit export set wurde
{
	int		j;
	char	*var;
	int		len;
	char	*value;

	ws = *i;
	(*i)++;
	j = 0;
	var = NULL;
	len = 0;
	value = NULL; //wenn value später immernoch NULL ist wurde sie nicht gefunden
	if (main->line[*i] && main->line[*i] == '?')
	{
		if (special_character(main, i, value, gc_list) < 0)
			return (-1);
	}
	else
	{
		if (expand_helper(main, i, ws, gc_list) < 0)
			return (-1);
	}
	return (0);
}


	// else if (main->line[*i] && main->line[*i] == '$')
	// {
	// 	value = ft_itoa(getpid()); // mistake
	// 	if (!value)
	// 		return (-1);
	// 	main->error = create_token(&main->tokens, TOKEN_WORD, value, gc_list);
	// 	(*i)++;
	// 	if (value)
	// 		free(value);
	// }
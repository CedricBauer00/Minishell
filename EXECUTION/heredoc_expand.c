/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:28:10 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/12 13:03:21 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*extract_expanded_env(char *line, int *i, t_shell *shell, t_gc *gc)
{
	char	*value;
	char	*key;
	int		idx;
	int		start;

	start = 0;
	idx = -1;
	value = NULL;
	start = *i;
	(*i)++;
	while (line[*i] && (ft_isalpha(line[*i]) || line[*i] == '_'))
		(*i)++;
	key = gc_substr(line, start, *i - start, gc);
	idx = check_existing(shell->my_envp, &key[1]);
	if (idx >= 0)
		value = extract_value(shell->my_envp[idx]);
	else
		value = gc_strdup("", &gc->temp);
	return (value);
}

char	*non_expanded_case(char *line, char *result, int *i, t_gc *gc)
{
	char	str[2];

	str[0] = line[*i];
	str[1] = '\0';
	result = gc_strjoin(result, str, &gc->temp);
	(*i)++;
	return (result);
}

char	*expand_case_in_heredoc(char *line, t_shell *shell)
{
	t_gc	*gc;
	char	*expanded;
	int		i;
	char	*result;

	i = 0;
	gc = get_gc();
	result = gc_strdup("", &gc->temp);
	expanded = NULL;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0')
		{
			expanded = extract_expanded_env(line, &i, shell, gc);
			result = gc_strjoin(result, expanded, &gc->temp);
		}
		else
			result = non_expanded_case(line, result, &i, gc);
	}
	return (result);
}


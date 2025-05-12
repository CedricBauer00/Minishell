/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:54:17 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/12 16:41:44 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	heredoc_quote_helper(char *word)
{
	static int	quote = 0;
	static char	c;

	if (quote == 0)
	{
		c = *word;
		quote = 1;
	}
	else if (quote == 1)
	{
		if (*word == c)
			quote = 0;
		else
			return (1);
	}
	return (0);
}

char	*del_quote(char *word, int i, t_gc_list **gc_list)
{
	char	*str;
	char	*ret;
	t_shell	*shell;

	shell = get_shell();
	str = (char *)malloc(sizeof(char) * (ft_strlen(word) + 1));
	if (!str)
		return (NULL);
	while (*word)
	{
		if (*word == '\'' || *word == '"')
		{
			shell->heredoc_expandable = false;
			if (heredoc_quote_helper(word) == 1)
				str[i++] = *word;
		}
		else
			str[i++] = *word;
		word++;
	}
	str[i] = '\0';
	ret = gc_strdup(str, gc_list);
	if (!ret)
		return (free(str), NULL);
	return (free(str), ret);
}

int	heredoc(t_main *main, int i, t_gc_list **gc_list)
{
	int		j;
	int		end;
	char	*word;

	if (main->line[i + 2] == '\0')
	{
		main->error = create_token(&main->tokens, TOKEN_HEREDOC, NULL, gc_list);
		return (i + 2);
	}
	j = i + 2;
	while (main->line[j] && check_char(main, j, 0) == 1)
		j++;
	end = is_quote(main, j);
	word = gc_strndup(&main->line[j], end - j, gc_list);
	if (!word)
		return (-1);
	word = del_quote(word, 0, gc_list);
	main->error = create_token(&main->tokens, TOKEN_HEREDOC, word, gc_list);
	return (end);
}

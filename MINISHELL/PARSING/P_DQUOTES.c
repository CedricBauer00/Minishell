/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_dquotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 09:42:44 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/06 15:17:30 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// int dquotes(t_main *main, int *i, t_gc_list *gc_list) // "HEELLOO $PATH" not working - multiple $VARs - $VAR=... not working 
// {
// 	int		ws;
// 	char	*subst;

// 	(*i)++;
// 	ws = *i;
// 	main->word = gc_strdup("", gc_list);
// 	if (!main->word)
// 		return (-1);
// 	while (main->line[*i])
// 	{
// 		while (main->line[*i] && main->line[*i] != '"') // Find closing quote
// 		{
// 			if (main->line[*i] == '$')
// 			{
// 				subst = gc_strndup(&main->line[ws], *i - ws, gc_list);
// 				if (!subst)
// 					return (-1);
// 				main->word = gc_strjoin(main->word, subst, gc_list);
// 				if (expands(main, &ws, 0, gc_list) < 0)
// 					return (perror("ERROR\nExpand failed!\n"), -1);
// 				ws = *i;
// 			}
// 			// else if (main->line[*i] == '\\' && main->line[*i + 1])
// 			// {
// 			// 	subst = gc_strndup(&main->line[ws], *i - ws, gc_list);
// 			// 	main->word = gc_strjoin(main->word, subst, gc_list);
// 			// 	subst = gc_strndup(&main->line[*i + 1], 1, gc_list);
// 			// 	main->word = gc_strjoin(main->word, subst, gc_list);
// 			// 	*i += 2;
// 			// 	ws = *i;
// 			// }
// 			else
// 				(*i)++;
// 		}
// 		if (*i > ws)
// 		{
// 			subst = gc_strndup(&main->line[ws], *i - ws, gc_list);
// 			main->word = gc_strjoin(main->word, subst, gc_list);
// 		}
// 		if (main->line[*i] == '"')
// 			(*i)++;
// 		main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
	
	
	
	
// 		if (main->line[*i] == '"') // If closing quote is found
// 		{
// 			main->word = gc_strndup(main->line + ws, *i - ws, gc_list);
// 			if (!main->word)
// 				return (-1);
// 			// if (is_built_in(main) == 1)
// 			// 	main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
// 			else
// 				main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
// 			if (main->error < 0)
// 				return (-1);
// 			(*i)++; // Move past the closing quote
// 			return 0; // Successfully processed one quoted segment
// 		}
// 		else
// 			return (printf(RED"ERROR\nUnclosed quotes!\n"DEFAULT), 0);
// 	}
// 	return 0; // Return success if no errors occurred
// }

// ws = i + 1;
// i++;
// while (1)
// {
// 	while (main->line[i] && main->line[i] != '"')
// 		i++;
// 	if (main->line[i] == '"')
// 	{
// 		main->word = gc_strndup(main->line + ws, i - ws, gc_list);
// 		if (!main->word)
// 		{
// 			return (perror("ERROR\nAllocating main->word failed!\n"), all_free(&gc_list), -1);
// 			exit(1);
// 		}
// 		if (is_built_in(main) == 1)
// 			main->error = create_token(&main->tokens, TOKEN_BUILT_IN, main->word, gc_list);
// 		else
// 			main->error = create_token(&main->tokens, TOKEN_WORD, main->word, gc_list);
// 		if (main->error < 0)
// 		{
// 			return (perror("ERROR\nToken creation failed!\n"), all_free(&gc_list), -1);
// 			exit(1);
// 		}
// 		i++;
// 		t_gc_list *todelte = find_node(gc_list, main->word);
// 		// printf(RED"main->word : %p\n"DEFAULT, main->word);
// 		delete_node(&gc_list, todelte);
// 		break ;
// 		// printf(RED"enter\n"DEFAULT);
// 	}
// 	main->next_line = readline("> ");
// 	if (!main->next_line)
// 		return (perror("ERROR\nFailed!\n"), all_free(&gc_list), -1);
	
// 	main->new = gc_strjoin(main->line, main->next_line, gc_list);
// 	if (!main->new)
// 	{
// 		free(main->next_line);
// 		return (perror("ERROR\nAllocating main->new failed!\n"), all_free(&gc_list), -1);
// 	}
// 	main->line = gc_strjoin(main->line, main->next_line, gc_list);
// 	free(main->next_line);
// 	main->next_line = NULL;
// }
// return (0);

int	get_spaces(t_main *main, int *i, t_gc_list *gc_list)
{
	int		j;
	int		k;
	char	*str;
	
	j = 0;
	k = 0;
	while (ft_isspace(main->line[*i]))
	{
		j++;
		(*i)++;
	}
	str = do_alloc(gc_list, j + 1, TYPE_SINGLE_PTR);
	if (!str)
		return (-1);
	str[j] = '\0';
	while (k < j)
	{
		str[k] = ' ';
		k++;
	}
	main->error = create_token(&main->tokens, TOKEN_WORD, str, gc_list);
	return (0);
}

int	read_word(t_main *main, int *i, t_gc_list *gc_list)
{
	int		j;
	int		k;
	int		m;
	char	*str;

	j = 0;
	k = 0;
	m = *i;
	while (main->line[m] && main->line[m] != '"' && !ft_isspace(main->line[m]) && main->line[m] != '$')
	{
		j++;
		m++;
	}
	str = do_alloc(gc_list, j + 1, TYPE_SINGLE_PTR);
	if (!str)
		return (-1);
	str[j] = '\0';
	while (k < j)
	{
		str[k] = main->line[*i];
		k++;
		(*i)++;
	}
	main->error = create_token(&main->tokens, TOKEN_WORD, str, gc_list);
	return (0);
}

int dquotes(t_main *main, int *i, t_gc_list *gc_list) //- $VAR=... not working 
{
	int ws;
	
	(*i)++;
	ws = *i;
	while (main->line[*i] && main->line[*i] != '"')
	{
		if (main->line[*i] == ' ')
		{
			if (get_spaces(main, i, gc_list) < 0)
				return (-1);
		}
		if (main->line[*i] == '$')
		{
			if (expands(main, i, 0, gc_list) < 0)
				return (perror("ERROR\nExpand failed!\n"), -1);
			while (main->line[*i] && !ft_isspace(main->line[*i]))
				(*i)++;
		}
		if (main->line[*i] && main->line[*i] != '"' && !ft_isspace(main->line[*i]) && main->line[*i] != '$')
		{
			if (read_word(main, i, gc_list) < 0)
				return (-1);
		}
	}
	if (main->line[*i] != '"')
		return (printf(RED"ERROR\nUnclosed quotes!\n"DEFAULT), 0);
	
	return (0);
}


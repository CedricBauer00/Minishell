/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:19:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/09 19:21:49 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
// #include <stdio.h>

// char	*ft_strndup(const char *str, size_t n)
// {
// 	size_t	len;
// 	size_t	counter;
// 	char	*ptr;

// 	len = ft_strlen(str);
// 	if (n < len)
// 		len = n;
// 	ptr = malloc((len + 1) * sizeof(char));
// 	if (!ptr)
// 		return (NULL);
// 	counter = 0;
// 	while (counter < len && str[counter] != '\0')
// 	{
// 		ptr[counter] = str[counter];
// 		counter++;
// 	}
// 	ptr[counter] = '\0';
// 	return (ptr);
// }

int	append_token(t_token **tokens, t_token *new_token) //create as double linked list
{
	t_token	*temp;
	
	if (!new_token)
		return (-1);
	if (!*tokens)
	{
		new_token->next = NULL;
		new_token->prev = NULL;
		*tokens = new_token;
		return (0);
	}
	temp = *tokens;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
	new_token->prev = temp;
	return (0);
}

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

int is_quote(t_main *main, int i)
{
	int		j;
	char	c;
	
	j = 0;
	while (1)
	{
		if (main->line[i] == '\0')
			return (i);
		else if (j == 1 && main->line[i] == c)
			j = 0;
		else if (j == 0 && (main->line[i] == '"' || main->line[i] == '\''))
		{
			c = main->line[i];
			j = 1;
		}
		else if (j == 0 && (main->line[i] == '|' || main->line[i] == '<' || main->line[i] == '>'))
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

void	free_tokens(t_token *tokens)
{
	t_token *temp;
	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

int	node_spaces_helper(t_token *temp, t_gc_list *gc_list)
{
	temp->value = gc_strjoin(temp->value, temp->next->value, gc_list);
	if (temp->value)
		return (-1);
	temp->next = temp->next->next;
	return (0);
}

int	check_for_node_spaces(t_main *main, t_token *temp, t_gc_list *gc_list)
{
	if (temp->type == TOKEN_SPACES)
	{
		main->tokens = temp->next;
		temp = temp->next;
	}	 
	while (temp)
	{
		if (temp->type == TOKEN_SPACES)
		{
			temp->prev->next = temp->next;
			if (temp->next != NULL)
				temp->next->prev = temp->prev;
			temp = temp->next;
		}
		else if (temp->next != NULL && temp->next->type != TOKEN_SPACES)
		{
			write(1, "else if\n", 8);
			while (temp->next != NULL && temp->next->type != TOKEN_SPACES)
			{
				if (node_spaces_helper(temp, gc_list) < 0)
					return (-1);
			}
			if (temp->next != NULL)
				temp->next->prev = temp;
			temp = temp->next;
		}
		else
			temp = temp->next;
	}
	return (0);
}

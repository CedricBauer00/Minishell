/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_token_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:30:51 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/20 12:26:03 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	node_spaces_helper(t_token *temp, t_gc_list *gc_list)
{
	temp->value = gc_strjoin(temp->value, temp->next->value, gc_list);
	if (!temp->value)
		return (-1);
	temp->next = temp->next->next;
	return (0);
}

int	node_space_else_if(t_token **temp, t_gc_list *gc_list)
{
	while ((*temp)->next != NULL && (*temp)->next->type != TOKEN_SPACES
		&& (*temp)->next->type != TOKEN_PIPE
		&& (*temp)->next->type != TOKEN_REDIRECT_IN
		&& (*temp)->next->type != TOKEN_REDIRECT_OUT
		&& (*temp)->next->type != TOKEN_HEREDOC
		&& (*temp)->next->type != TOKEN_APPEND
		&& (*temp)->next->type != TOKEN_PIPE)
	{
		if (node_spaces_helper(*temp, gc_list) < 0)
			return (-1);
	}
	if ((*temp)->next != NULL)
		(*temp)->next->prev = (*temp);
	(*temp) = (*temp)->next;
	return (0);
}

int	check_tokens(t_token *temp)
{
	if (temp->next != NULL && temp->type != TOKEN_APPEND
		&& temp->type != TOKEN_HEREDOC
		&& temp->type != TOKEN_REDIRECT_IN
		&& temp->type != TOKEN_REDIRECT_OUT
		&& temp->type != TOKEN_PIPE
		&& temp->next->type != TOKEN_SPACES)
		return (0);
	return (-1);
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
		else if (check_tokens(temp) == 0)
		{
			if (node_space_else_if(&temp, gc_list) < 0)
				return (-1);
		}
		else
			temp = temp->next;
	}
	return (0);
}

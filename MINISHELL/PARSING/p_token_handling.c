/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:30:51 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/15 14:48:48 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

int	create_token(t_token **tokens, t_token_type type, char *str, t_gc_list *gc_list)
{
	t_token *new_token;
	
	new_token = (t_token *)do_alloc(&gc_list, sizeof(t_token),TYPE_SINGLE_PTR, "create_token");
	if (!new_token)
		return (perror("ERROR\nMalloc failed!\n"), -1);
	new_token->type = type;
	new_token->value = gc_strdup(str, gc_list);
	if (!new_token->value && str != NULL)
	{
		free(new_token);
		return (perror("ERROR\nMalloc for new_token->value failed!\n"), -1);
	}
	new_token->next = NULL;
	return (append_token(tokens, new_token));
}

void	free_tokens(t_token *tokens) //maybe not needed
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
	if (!temp->value)
		return (-1);
	temp->next = temp->next->next;
	return (0);
}

int	node_space_else_if(t_token **temp, t_gc_list *gc_list)
{
	write(1, "else if\n", 8);
	while ((*temp)->next != NULL && (*temp)->next->type != TOKEN_SPACES
		&& (*temp)->next->type != TOKEN_PIPE
		&& (*temp)->next->type != TOKEN_REDIRECT_IN
		&& (*temp)->next->type != TOKEN_REDIRECT_OUT
		&& (*temp)->next->type != TOKEN_HEREDOC
		&& (*temp)->next->type != TOKEN_APPEND)
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

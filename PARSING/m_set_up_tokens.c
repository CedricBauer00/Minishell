/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_set_up_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:26:06 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/14 13:49:16 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	append_token(t_token **tokens, t_token *new_token)
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

int	create_token(t_token **tokens, t_token_type type, char *str, \
	t_gc_list **gc_list)
{
	t_token	*new_token;

	new_token = (t_token *)do_alloc(gc_list, sizeof(t_token), \
		TYPE_SINGLE_PTR, "create_token");
	if (!new_token)
		return (-1);
	new_token->type = type;
	new_token->value = gc_strdup(str, gc_list);
	if (!new_token->value && str != NULL)
		return (-1);
	new_token->next = NULL;
	return (append_token(tokens, new_token));
}

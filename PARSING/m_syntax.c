/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_syntax.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:01:32 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/02 17:33:44 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	syntax_redirects(t_token **cur)
{
	if ((*cur)->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND))
	{
		if ((*cur)->next && (*cur)->next->type == TOKEN_HEREDOC)
			return (choose_error_statement(2, "<<"), -1);
		else if ((*cur)->next && (*cur)->next->type != TOKEN_FILE)
			return (choose_error_statement(2, (*cur)->next->value), -1);
	}
	else if ((*cur)->type == TOKEN_HEREDOC)
	{
		if ((*cur)->next == NULL && ((*cur)->value == NULL
				|| (*cur)->value[0] == '\0'))
			return (choose_error_statement(1, NULL), -1);
		else if ((*cur)->next != NULL && (*cur)->next->type == TOKEN_HEREDOC \
			&& ((*cur)->value == NULL || (*cur)->value[0] == '\0'))
			return (choose_error_statement(2, "<<"), -1);
		else if ((*cur)->value == NULL || (*cur)->value[0] == '\0')
			return (choose_error_statement(2, (*cur)->next->value), -1);
	}
	return (0);
}

int	syntax_helper(t_token **cur)
{
	if (((*cur)->type & (TOKEN_APPEND | TOKEN_REDIRECT_IN
				| TOKEN_REDIRECT_OUT)) && (!(*cur)->next))
		return (choose_error_statement(1, NULL), -1);
	else if ((*cur)->type == TOKEN_PIPE || (*cur)->type == TOKEN_BONUS)
	{
		if ((*cur)->type == TOKEN_PIPE && (!(*cur)->prev || !(*cur)->next))
			return (choose_error_statement(2, (*cur)->value), -1);
		else if (((*cur)->next != NULL && (*cur)->next->type == TOKEN_PIPE)
			|| (*cur)->type == TOKEN_BONUS)
			return (choose_error_statement(3, NULL), -1);
	}
	if (syntax_redirects(cur) < 0)
		return (-1);
	return (0);
}
//int	validate_syntax(t_token *token, t_gc_list *gc_list)
int	validate_syntax(t_token *token)
{
	t_token	*cur;

	if (!token)
		return (1);
	cur = token;
	while (cur)
	{
		if (syntax_helper(&cur) < 0)
		{
			get_shell()->last_status_exit = 258;
			return (-1);
		}
		cur = cur->next;
	}
	return (1);
}

/* 
	<< | should be iinvalid
	< null
	> null
	>> null
	null | null
	| null
	null |
	|| 
	if || valid -> bonus_error
	if && valid -> bonus_error
*/
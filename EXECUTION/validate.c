/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:35 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 10:04:41 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	validate_syntax(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if ((cur->type & (TOKEN_PIPE | TOKEN_APPEND | TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT)) &&
			(!cur->next || cur->next->type == TOKEN_PIPE || cur->next->type == TOKEN_NONE))
			perror(RED"syntax error"DEFAULT);
		else if (cur->type == TOKEN_PIPE)
		{
			if(!cur->prev || cur->next->type == TOKEN_PIPE)
				perror(RED"syntax error"DEFAULT);
		}
		else if (cur->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND))
		{
			if (!cur->next || cur->next->type != TOKEN_FILE)
				perror(RED"syntax error"DEFAULT);
		}
		else if (cur->type == TOKEN_HEREDOC)
		{
			if(cur->value == NULL)
				perror(RED"syntax error"DEFAULT);
		}
		cur = cur->next;
	}
	return 1;
}

bool	is_validate_cmd_block(t_cmd_block *cmd_b)
{
	t_cmd_block	*cur;

	cur = cmd_b;
	while(cur)
	{
		if (cur->built_in && cur->args)
			return false;
		cur = cur->next;
	}
	return true;
}

void	validate_check(t_cmd_block *cmd_block)
{
	t_shell		*shell;
	t_cmd_block	*cur;
	t_gc		*gc;

	gc = get_gc();
	shell =get_shell();
	cur = cmd_block;
	if (!cmd_block || is_validate_cmd_block(cur) == false)
	{
		gc_free(gc);
		perror(RED"non valid cmd"DEFAULT);
		shell->last_status_exit = 1;
		exit(1);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouplize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:08 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/17 12:40:36 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_cmd_block	*grouplize(t_token *token, t_cmd_block **cmd_block, t_gc *gc)
{
	t_cmd_block	*new_cmd_block;
	t_cmd_block	*last;

	if (!token)
		return (NULL);
	while (token != NULL)
	{
		if (token && token->type == TOKEN_PIPE)
			token = token->next;
		new_cmd_block = merge_to_one_cmd(&token, gc);
		is_exited((t_cmd_block *)new_cmd_block, gc);
		if (*cmd_block == NULL)
			*cmd_block = new_cmd_block;
		else
		{
			last = *cmd_block;
			while (last->next)
				last = last->next;
			last->next = new_cmd_block;
			new_cmd_block->prev = last;
		}
	}
	return (*cmd_block);
}

t_cmd_block	*merge_to_one_cmd(t_token **token, t_gc *gc)
{
	t_cmd_block	*new_cmd_block;
	t_token		*cur;
	int			args_count;
	int			i;

	i = 0;
	args_count = 0;
	cur = *token;
	new_cmd_block = init_command_struct(gc);
	is_exited((t_cmd_block *)new_cmd_block, gc);
	args_count = count_cmd_block(cur);
	new_cmd_block->args = (char **)do_alloc(&gc->temp, \
		sizeof(char *) * (args_count + 1), TYPE_DOUBLE_PTR, "args");
	is_exited(new_cmd_block->args, gc);
	ready_all(new_cmd_block, &cur, gc, &i);
	new_cmd_block->args[args_count] = NULL;
	*token = cur;
	return (new_cmd_block);
}

void	ready_all(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc, int *i)
{
	while (*cur && (*cur)->type != TOKEN_PIPE)
	{
		if (!cur || !*cur || !new_cmd_block || !i)
			return ;
		if (*cur && ((*cur)->type & (TOKEN_REDIRECT_IN \
			| TOKEN_REDIRECT_OUT | TOKEN_APPEND | TOKEN_HEREDOC)))
		{
			add_io_streams(cur, new_cmd_block);
			continue ;
		}
		if (cur && (*cur)->type == TOKEN_BUILT_IN)
			ready_builtin(new_cmd_block, cur, gc);
		else if (*cur && (*cur)->type == TOKEN_ARG)
			ready_args(new_cmd_block, cur, gc, i);
		(*cur) = (*cur)->next;
	}
}

int	count_cmd_block(t_token *token)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = token;
	while (temp && temp->type != TOKEN_PIPE)
	{
		if (temp && temp->type == TOKEN_ARG)
			i++;
		temp = temp ->next;
	}
	return (i);
}

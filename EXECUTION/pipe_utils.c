/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:18 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/15 14:50:37 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	add_pipe(t_cmd_block **cmd_block)
{
	t_pipe	*new_pipe_node;
	t_gc	*gc;

	gc = get_gc();
	if (!cmd_block || !*cmd_block)
		return ;
	new_pipe_node = init_pipe(gc);
	fprintf(stderr, "pipe()created \n");
	if (!new_pipe_node)
	{
		gc_free(gc);
		exit(1);
	}
	if ((*cmd_block)->pipe == NULL)
		(*cmd_block)->pipe = new_pipe_node;
}

bool	is_first_pipe(t_cmd_block *cmd)
{
	if (!cmd->prev && cmd->next)
		return (true);
	else
		return (false);
}

bool	is_middle_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && cmd->next)
		return (true);
	else
		return (false);
}

bool	is_last_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && !cmd->next)
		return (true);
	else
		return (false);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouplize_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:43:28 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/12 15:14:57 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	add_io_streams_list(t_io_streams_list **head, \
	t_io_streams_list *new_io_streams)
{
	t_io_streams_list	*last;

	if (!*head)
		*head = new_io_streams;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new_io_streams;
	}
}

void	ready_redir_files(t_io_streams_list *new_io_streams, \
	t_token **cur, t_gc *gc)
{
	t_gc_list	*find;

	if (!cur || !*cur || !(*cur)->next || (*cur)->next->type != TOKEN_FILE)
		return ;
	if ((*cur)->type == TOKEN_REDIRECT_IN)
		new_io_streams->infile_name = \
			gc_strdup((*cur)->next->value, &gc->temp);
	else if ((*cur)->type == TOKEN_REDIRECT_OUT)
		new_io_streams->outfile_name = \
			gc_strdup((*cur)->next->value, &gc->temp);
	else if ((*cur)->type == TOKEN_APPEND)
		new_io_streams->append_file_name = \
			gc_strdup((*cur)->next->value, &gc->temp);
	find = find_node(gc->temp, (char *)(*cur)->next->value);
	delete_node(&gc->temp, find);
	*cur = (*cur)->next;
}

void	ready_builtin(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc)
{
	t_gc_list	*find;

	if (!cur || !*cur || !new_cmd_block)
		return ;
	new_cmd_block->is_built_in = true;
	new_cmd_block->built_in = gc_strdup((*cur)->value, &gc->temp);
	find = find_node(gc->temp, (char *)(*cur)->value);
	delete_node(&gc->temp, find);
}

void	add_io_streams(t_token **cur, t_cmd_block *new_cmd_block)
{
	t_io_streams_list	*new_io_streams;
	t_gc				*gc;
	t_gc_list			*find;

	new_io_streams = NULL;
	gc = get_gc();
	if (!cur)
		return ;
	new_io_streams = init_io_stream_struct(gc);
	is_exited((t_io_streams_list *)new_io_streams, gc);
	add_io_streams_list(&new_cmd_block->io_streams, new_io_streams);
	if ((*cur)->type & (TOKEN_HEREDOC))
	{
		new_io_streams->heredoc_eof = gc_strdup((*cur)->value, &gc->temp);
		find = find_node(gc->temp, (char *)(*cur)->value);
		delete_node(&gc->temp, find);
		*cur = (*cur)->next;
	}
	if (*cur && (*cur)->next && (*cur)->next->type == TOKEN_FILE)
		ready_redir_files(new_io_streams, cur, gc);
}

void	ready_args(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc, int *i)
{
	t_gc_list	*find;

	if (!cur || !*cur || !new_cmd_block || !i)
		return ;
	if (*i == 0 && !new_cmd_block->is_built_in
		&& !(new_cmd_block->io_streams && !(*cur)->next))
		new_cmd_block->is_external_cmd = true;
	new_cmd_block->args[(*i)++] = gc_strdup((*cur)->value, &gc->temp);
	find = find_node(gc->temp, (char *)(*cur)->value);
	delete_node(&gc->temp, find);
}

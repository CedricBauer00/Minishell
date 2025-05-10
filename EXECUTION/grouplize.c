/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouplize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:08 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 15:29:37 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	grouplize(t_token *token, t_cmd_block **cmd_block, t_gc *gc)
{
	if (!token)
		return ;
	while (token != NULL)
	{
		if(token && token->type == TOKEN_PIPE)
			token = token->next;
		t_cmd_block *new_cmd_block = merge_to_one_cmd(&token, gc);
		is_exited((t_cmd_block*)new_cmd_block, gc);
		if (*cmd_block == NULL)
		{
			*cmd_block = new_cmd_block;
		}
		else
		{
			t_cmd_block *last = *cmd_block;
			while(last->next)
			{
				last = last->next;
			}
			last->next = new_cmd_block;
			new_cmd_block->prev = last;
		}
	}
}

static int	count_cmd_block(t_token *token)
{
	t_token *temp;
	int	i;

	i = 0;
	temp = token;
	while(temp && temp->type != TOKEN_PIPE)
	{
		if (temp && temp->type == TOKEN_ARG)
		{
			i++;
		}
		temp = temp ->next;
	}
	return i;
}

static void add_io_streams_list(t_io_streams_list **head, t_io_streams_list *new_io_streams)
{
	if(!*head)
		*head = new_io_streams;
	else
	{
		t_io_streams_list *last;
		last = *head;
		while(last->next)
		{
			last = last->next;
		}
		last->next = new_io_streams;
	}
}

static void ready_redir_files(t_io_streams_list *new_io_streams, t_token **cur, t_gc *gc)
{
	if (!cur || !*cur || !(*cur)->next || (*cur)->next->type != TOKEN_FILE)
		return;
	if ((*cur)->type == TOKEN_REDIRECT_IN)
			new_io_streams->infile_name = gc_strdup((*cur)->next->value, &gc->temp);
	else if ((*cur)->type == TOKEN_REDIRECT_OUT)
		new_io_streams->outfile_name = gc_strdup((*cur)->next->value, &gc->temp);
	else if ((*cur)->type == TOKEN_APPEND)
		new_io_streams->append_file_name = gc_strdup((*cur)->next->value, &gc->temp);
	t_gc_list *find;
	find = find_node(gc->temp, (char*)(*cur)->next->value);
	delete_node(&gc->temp, find);
	*cur = (*cur)->next;
}

static void ready_builtin(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc)
{
	if (!cur || !*cur || !new_cmd_block)
		return ;
	new_cmd_block->is_built_in = true;
	new_cmd_block->built_in = gc_strdup((*cur)->value, &gc->temp);
	t_gc_list *find;
	find = find_node(gc->temp, (char*)(*cur)->value);
	delete_node(&gc->temp, find);
}

static void	add_io_streams(t_token **cur, t_cmd_block *new_cmd_block)
{
	t_io_streams_list *new_io_streams;
	t_gc *gc;

	new_io_streams = NULL;
	gc = get_gc();
	if (!cur)
		return ;
	
	new_io_streams = init_io_stream_struct(gc);
	is_exited((t_io_streams_list*)new_io_streams, gc);
	add_io_streams_list(&new_cmd_block->io_streams, new_io_streams);
	if((*cur)->type & (TOKEN_HEREDOC))
	{
		new_io_streams->heredoc_eof = gc_strdup((*cur)->value, &gc->temp);
		t_gc_list *find;
		find = find_node(gc->temp, (char*)(*cur)->value);
		delete_node(&gc->temp, find);
		*cur = (*cur)->next;
	}
	if(*cur && (*cur)->next && (*cur)->next->type == TOKEN_FILE)
	{
		ready_redir_files(new_io_streams, cur, gc);
	}
}

static void ready_args(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc, int *i)
{
	if (!cur || !*cur || !new_cmd_block || !i)
		return ;
	if (*i == 0 && !new_cmd_block->is_built_in && !(new_cmd_block->io_streams && !(*cur)->next))
	{
		new_cmd_block->is_external_cmd = true;
	}
	new_cmd_block->args[(*i)++] = gc_strdup((*cur)->value, &gc->temp);
	
	t_gc_list *find;
	find = find_node(gc->temp, (char*)(*cur)->value);
	delete_node(&gc->temp, find);
}

static void ready_all(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc, int *i)
{	
	while(*cur && (*cur)->type != TOKEN_PIPE)
	{
		if (!cur || !*cur || !new_cmd_block || !i)
			return ;
		if (*cur && ((*cur)->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND | TOKEN_HEREDOC)))
		{
			add_io_streams(cur, new_cmd_block);
			continue;
		}
		if (cur && (*cur)->type == TOKEN_BUILT_IN)
			ready_builtin(new_cmd_block, cur ,gc);
		else if (*cur && (*cur)->type == TOKEN_ARG)
			ready_args(new_cmd_block, cur ,gc, i);
		(*cur) = (*cur)->next;
	}
}

t_cmd_block	*merge_to_one_cmd(t_token **token, t_gc *gc)
{
	t_cmd_block			*new_cmd_block;
	t_token				*cur;
	int args_count = 0;

	cur = *token;
	new_cmd_block = init_command_struct(gc);
	is_exited((t_cmd_block*)new_cmd_block, gc);
	int i = 0;
	args_count = count_cmd_block(cur);
	new_cmd_block->args = (char**)do_alloc(&gc->temp, sizeof(char*) * (args_count + 1), TYPE_DOUBLE_PTR, "new_cmd_block->args");
	is_exited(new_cmd_block->args, gc);
	
	ready_all(new_cmd_block, &cur, gc , &i);
	new_cmd_block->args[args_count] = NULL;
	*token = cur;
	return new_cmd_block;
}

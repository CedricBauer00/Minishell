/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouplize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:49:42 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/05 09:55:44 by cbauer           ###   ########.fr       */
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

t_cmd_block	*merge_to_one_cmd(t_token **token, t_gc *gc)
{
	t_cmd_block			*new_cmd_block;
	t_io_streams_list	*new_io_streams;
	t_token				*cur;
	int					i;
	
	i = 0;
	cur = *token;
	t_token *temp = cur;
	new_cmd_block = init_command_struct(gc);
	is_exited((t_cmd_block*)new_cmd_block, gc);
	while(temp && temp->type != TOKEN_PIPE)
	{
		if (temp && temp->type == TOKEN_ARG)
		{
			i++;
		}
		temp = temp ->next;
	}
	new_cmd_block->args = (char**)do_alloc(&gc->temp, sizeof(char*) * (i + 1), TYPE_DOUBLE_PTR, "new_cmd_block->args");
	is_exited(new_cmd_block->args, gc);
	i = 0;
	while(cur && cur->type != TOKEN_PIPE)
	{
		if (cur && (cur->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND | TOKEN_HEREDOC)))  //token->value : < filename
		{
			new_io_streams = init_io_stream_struct(gc->temp);
			is_exited((t_io_streams_list*)new_io_streams, gc);
			if(!new_cmd_block->io_streams)
				new_cmd_block->io_streams = new_io_streams;
			else
			{
				t_io_streams_list *last;
				last = new_cmd_block->io_streams;
				while(last->next)
				{
					last = last->next;
				}
				last->next = new_io_streams;
			}
			if(cur->type & (TOKEN_HEREDOC))
			{
				fprintf(stderr, RED"if heredoc in grouplize()\n"DEFAULT);
				new_io_streams->heredoc_eof = gc_strdup(cur->value, &gc->temp);
				t_gc_list *find;
				find = find_node(gc->temp, (char*)cur->value);
				delete_node(&gc->temp, find);
			}
			if(cur->next && cur->next->type == TOKEN_FILE)
			{
				if (cur->type == TOKEN_REDIRECT_IN)
					new_io_streams->infile_name = gc_strdup(cur->next->value, &gc->temp);
				else if (cur->type == TOKEN_REDIRECT_OUT)
					new_io_streams->outfile_name = gc_strdup(cur->next->value, &gc->temp);
				else if (cur->type == TOKEN_APPEND)
					new_io_streams->append_file_name = gc_strdup(cur->next->value, &gc->temp);
				t_gc_list *find;
				find = find_node(gc->temp, (char*)cur->next->value);
				delete_node(&gc->temp, find);
				cur = cur->next;
				continue;
			}
		}
		if (cur && cur->type == TOKEN_BUILT_IN)
		{
			new_cmd_block->is_built_in = true;
			new_cmd_block->built_in = gc_strdup(cur->value, &gc->temp);
			t_gc_list *find;
			find = find_node(gc->temp, (char*)cur->value);
			delete_node(&gc->temp, find);
		}
		else if (cur && cur->type == TOKEN_ARG)
		{
			if (i == 0 && !new_cmd_block->is_built_in && !(new_cmd_block->io_streams && !cur->next))
			{
				new_cmd_block->is_external_cmd = true;
			}
			new_cmd_block->args[i++] = gc_strdup(cur->value, &gc->temp);
			t_gc_list *find;
			find = find_node(gc->temp, (char*)cur->value);
			delete_node(&gc->temp, find);
		}
		new_cmd_block->args[i] = NULL;
		cur = cur->next;
	}
	*token = cur;
	return new_cmd_block;
}


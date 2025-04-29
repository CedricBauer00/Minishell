/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:02:03 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/20 14:16:17 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

t_gc	*get_gc(void)
{
	static t_gc	*gc = {0};

	if (!gc)
		gc = init_gc();
	return (gc);
}

t_gc	*init_gc(void)
{
	t_gc	*gc;

	gc = malloc(sizeof(t_gc));
	if (!gc)
	{
		return (NULL);
	}
	gc->temp = init_gc_list();
	if (!gc->temp)
		free(gc);
	gc->shell = init_gc_list();
	if (!gc->shell)
	{
		free(gc->temp);
		free(gc);
		return (NULL);
	}
	return (gc);
}

t_gc_list	*init_gc_list(void)
{
	t_gc_list	*head;

	head = malloc(sizeof(t_gc_list));
	if (!head)
	{
		return (NULL);
	}
	head->data = NULL;
	head->next = NULL;
	head->type = 0;
	head->id = 0;
	return (head);
}

void	*do_alloc(t_gc_list **gc_lst, size_t howmuch, \
	t_data_type data_type, char *id)
{
	t_gc_list	*new_node;
	void		*data;

	new_node = malloc(sizeof(t_gc_list));
	if (!new_node)
	{
		return (NULL);
	}
	data = malloc(howmuch);
	if (!data)
	{
		free (new_node);
		return (NULL);
	}
	new_node->data = data;
	new_node->next = NULL;
	new_node->type = data_type;
	new_node->id = id;
	new_node->level = (*gc_lst)->level;
	if (*gc_lst)
		(*gc_lst)->next = new_node;
	return (data);
}

void	free_data_by_type(void *data, t_data_type data_type)
{
	char	**temp;

	if (!data)
		return ;
	if (data_type == TYPE_SINGLE_PTR)
	{
		free(data);
		data = NULL;
	}
	else if (data_type == TYPE_DOUBLE_PTR)
	{
		temp = (char **)data;
		free (temp);
		temp = NULL;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:00:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/14 16:12:40 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#include "garbage_collector.h"

t_gc_list	*find_node(t_gc_list *gc_lst, void *target)
{
	t_gc_list	*cur;

	cur = gc_lst;
	while (cur)
	{
		if (cur->data == target)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

t_gc_list	*find_node_with_id(t_gc_list *gc_lst, char *id)
{
	t_gc_list	*cur;

	cur = gc_lst;
	while (cur)
	{
		if (ft_strncmp(cur->id, id, ft_strlen(cur->id)) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

void	delete_node(t_gc_list **gc_lst, t_gc_list *to_delete)
{
	t_gc_list	*cur;
	t_gc_list	*prev;

	if (!gc_lst || !(*gc_lst))
		return ;
	prev = NULL;
	cur = (*gc_lst);
	while (cur)
	{
		if (cur == to_delete)
		{
			free_data_by_type(cur->data, cur->type);
			if (prev == NULL)
				*gc_lst = cur->next;
			else
				prev->next = cur->next;
			free(cur);
			cur = NULL;
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
}

void	all_free(t_gc_list **gc_lst)
{
	t_gc_list	*cur;
	t_gc_list	*next;

	if (!gc_lst || !(*gc_lst))
		return ;
	cur = (*gc_lst);
	while (cur)
	{
		next = cur->next;
		free_data_by_type(cur->data, cur->type);
		free(cur);
		cur = next;
	}
	*gc_lst = NULL;
}

void	gc_level_up(t_gc_list *gc_list)
{
	gc_list->level++;
}

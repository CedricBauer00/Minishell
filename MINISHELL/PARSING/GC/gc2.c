/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:00:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/01 13:10:31 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

t_gc_list	*find_node(t_gc_list *gc_lst, void *target)
{
	t_gc_list	*cur;

	cur = gc_lst->next;
	while (cur)
	{
		if (cur->data == target)
		{
			printf("----------find : %p in garbage--------\n", target);
			return (cur);
		}
		cur = cur->next;
	}
	return (NULL);
}

t_gc_list	*find_node_with_id(t_gc_list *gc_lst, char *id)
{
	t_gc_list	*cur;

	cur = gc_lst->next;
	while (cur)
	{
		if (ft_strncmp(cur->id, id, ft_strlen(cur->id)) == 0)
		{
			printf("----------find : %s in garbage--------\n", id);
			return (cur);
		}
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
	prev = *gc_lst;
	cur = (*gc_lst)->next;
	while (cur)
	{
		if (cur == to_delete)
		{
			printf("Deleting Node: %p, Data: %p, Type: %d\n", cur, cur->data, cur->type);
			free_data_by_type(cur->data, cur->type);
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
	cur = (*gc_lst)->next;
	printf("*gc_list %p\n", (*gc_lst));
	while (cur)
	{
		next = cur->next;
        printf("Freeing Node: %p, Data: %p, Type: %d, ID: %s\n", cur, cur->data, cur->type, cur->id);
		free_data_by_type(cur->data, cur->type);
		free(cur);
		cur = next;
	}
	//*gc_lst = NULL;
}

void	gc_level_up(t_gc_list *gc_list)
{
	gc_list->level++;
}

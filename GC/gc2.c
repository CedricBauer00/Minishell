/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:00:46 by cbauer            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/02 17:30:45 by cbauer           ###   ########.fr       */
=======
/*   Updated: 2025/05/03 13:10:12 by jisokim2         ###   ########.fr       */
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

t_gc_list	*find_node(t_gc_list *gc_lst, void *target)
{
	t_gc_list	*cur;

	cur = gc_lst;
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

	cur = gc_lst;
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
	prev = NULL;
	cur = (*gc_lst);
	while (cur)
	{
		if (cur == to_delete)
		{
			//printf("Deleting Node: %p, Data: %p, Type: %d\n", cur, cur->data, cur->type);
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
	cur = (*gc_lst);
	while (cur)
	{
		next = cur->next;
		free_data_by_type(cur->data, cur->type);
<<<<<<< HEAD
		// printf(PURPLE"Freeing Node: %p, Data: %p, Type: %d, ID: %s\n"RESET, 
			// cur, cur->data, cur->type, cur->id);
=======
		//printf(PURPLE"Freeing Node: %p, Data: %p, Type: %d, ID: %s\n"RESET, cur, cur->data, cur->type, cur->id);
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
		free(cur);
		cur = next;
	}
	*gc_lst = NULL;
}

void	gc_level_up(t_gc_list *gc_list)
{
	gc_list->level++;
}

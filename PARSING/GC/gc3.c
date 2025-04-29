/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:01:21 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/20 14:20:09 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

void	gc_free_by_level(t_gc_list *gc_list)
{
	t_gc_list	*cur;
	t_gc_list	*prev;
	t_gc_list	*to_free;

	prev = gc_list;
	cur = gc_list->next;
	while (cur)
	{
		if (cur->level == gc_list->level)
		{
			to_free = cur;
			prev->next = cur->next;
			cur = cur->next;
			free_data_by_type(to_free->data, to_free->type);
			free(to_free);
			to_free = NULL;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
	gc_list->level--;
}

void	gc_free(t_gc *gc)
{
	if (!gc || !gc->temp || !gc->shell)
		return ;
	if (gc->temp)
	{
		all_free(&gc->temp);
		free(gc->temp);
		gc->temp = NULL;
	}
	if (gc->shell)
	{
		all_free(&gc->shell);
		free(gc->shell);
		gc->shell = NULL;
	}
	free(gc);
	gc = NULL;
}

void	print_list(t_gc_list *gc_lst)
{
	t_gc_list	*cur;
	int			i;

	i = 1;
	cur = gc_lst->next;
	while (cur != NULL)
	{
		printf("[%d]th NODE[%s], %p\n", i, cur->data, cur->data);
		i++;
		cur = cur->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:01:21 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/01 13:07:52 by cbauer           ###   ########.fr       */
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
			printf("Freeing Node by Level: %p, Data: %p, Type: %d, Level: %d\n", to_free, to_free->data, to_free->type, to_free->level);
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
	printf("Decreased GC Level to: %d\n", gc_list->level);
}

void	gc_free(t_gc *gc)
{
	if (!gc)
		return ;
	printf("call gc_free\n");
	if (gc->shell)
	{
		printf("call gc_free gc->shell %p\n", gc->shell);
		all_free(&gc->shell);
		free(gc->shell);
		gc->shell = NULL;
	}
	if (gc->temp)
	{
		printf("call gc_free gc->temp\n");
		all_free(&gc->temp);
		free(gc->temp);
		gc->temp = NULL;
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
		printf("[%d]th NODE[%p] [%s], %p\n", i, cur, (char*)cur->data, cur->data);
		i++;
		cur = cur->next;
	}
}

void	print_gc(t_gc *gc)
{
	t_gc	*cur;
	int			i;

	i = 1;
	cur = gc;
	while (cur->temp != NULL)
	{
		printf("[%d]th NODE[%s], %p\n", i, (char*)cur->temp->data, cur->temp->data);
		i++;
		cur->temp = cur->temp->next;
	}
}


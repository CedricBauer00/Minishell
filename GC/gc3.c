/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:01:21 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/09 17:06:19 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			// printf("Freeing Node by Level: %p, Data: %p, Type: %d, Level: %d\n", to_free, to_free->data, to_free->type, to_free->level);
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
	// printf("Decreased GC Level to: %d\n", gc_list->level);
}

void	gc_free(t_gc *gc)
{
	if (!gc)
		return ;
	if (gc->shell)
	{
		//print_list(&gc->shell);
		all_free(&gc->shell);
	}
	if (gc->temp)
	{
		//print_list(&gc->temp);
		all_free(&gc->temp);
	}
	//printf(PURPLE"free gc %p\n"RESET, gc);
	free(gc);
	gc = NULL;
	//printf(POWER_PURPLE"--------------------------------------GC CLEAN-------------------------------------------"RESET);
	// printf("\n");
}

void	print_list(t_gc_list **gc_lst)
{
	t_gc_list	*cur;
	int			i;

	i = 1;
	if (!gc_lst || !*gc_lst)
		return ;
	cur = (*gc_lst);
	while (cur != NULL)
	{
		printf(PURPLE"[%d]th NODE[%p] [%s], %p\n"RESET, i, cur, (char*)cur->data, cur->data);
		i++;
		cur = cur->next;
	}
}

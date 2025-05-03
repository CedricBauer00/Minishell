/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:01:21 by cbauer            #+#    #+#             */
<<<<<<< HEAD
<<<<<<< HEAD
/*   Updated: 2025/05/02 17:29:49 by cbauer           ###   ########.fr       */
=======
/*   Updated: 2025/05/03 13:10:30 by jisokim2         ###   ########.fr       */
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
=======
/*   Updated: 2025/05/03 13:10:30 by jisokim2         ###   ########.fr       */
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
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
	if (gc->shell)
	{
		print_list(&gc->shell);
		all_free(&gc->shell);
	}
	if (gc->temp)
	{
		print_list(&gc->temp);
		all_free(&gc->temp);
	}
	//printf(PURPLE"free gc %p\n"RESET, gc);
	free(gc);
	gc = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	// printf(POWER_PURPLE"--------------------------------------GC CLEAN-------------------------------------------"RESET);
=======
	//printf(POWER_PURPLE"--------------------------------------GC CLEAN-------------------------------------------"RESET);
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
=======
	//printf(POWER_PURPLE"--------------------------------------GC CLEAN-------------------------------------------"RESET);
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
	printf("\n");
}

void	print_list(t_gc_list **gc_lst)
{
	t_gc_list	*cur;
	int			i;

	i = 1;
	cur = (*gc_lst);
	while (cur != NULL)
	{
<<<<<<< HEAD
<<<<<<< HEAD
		// printf(PURPLE"[%d]th NODE[%p] [%s], %p\n"RESET, i, cur, (char*)cur->data, cur->data);
=======
		//printf(PURPLE"[%d]th NODE[%p] [%s], %p\n"RESET, i, cur, (char*)cur->data, cur->data);
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
=======
		//printf(PURPLE"[%d]th NODE[%p] [%s], %p\n"RESET, i, cur, (char*)cur->data, cur->data);
>>>>>>> 7245e98652b9aac3fc550ac8191a581d58889e87
		i++;
		cur = cur->next;
	}
}

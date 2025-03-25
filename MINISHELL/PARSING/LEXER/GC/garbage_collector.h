#ifndef GABAGE_COLLECTOR_H
# define GABAGE_COLLECTOR_H

//temp header
# include <stdio.h>
# include <stdlib.h>

typedef enum	e_data_type
{
	TYPE_SINGLE_CHAR,
	TYPE_DOUBLE_CHAR,
}	t_data_type;

typedef struct	s_gc_list
{
	void				*data;
	struct s_gc_list	*next;
	t_data_type			type;
	//int					ref_count;
}	t_gc_list;

//empty node, just a head node as dummy.

t_gc_list	*init_gc_list(void);
void	*do_alloc(t_gc_list *gc_lst, size_t howmuch, t_data_type data_type);
void	free_data_type(void *data, t_data_type data_type);
void	gc_free(t_gc_list **gc_lst);
void	print_list(t_gc_list *gc_lst);



// void	gc_inc_ref_count(t_gc_list *gc_lst, void *ptr)
// {
// 	t_gc_list *cur;

// 	cur = gc_lst->next;
// 	while(cur)
// 	{
// 		if(cur->data == ptr)
// 		{
// 			cur->ref_count++;
// 			printf("cur:%p, cur->ref_count:%d\n",cur->data, cur->ref_count);
// 			break;
// 		}
// 		cur = cur->next;
// 	}
// }

// void	gc_dec_ref_count(t_gc_list *gc_lst, void *ptr)
// {
// 	t_gc_list	*cur;
// 	t_gc_list	*prev;

// 	prev = gc_lst;
// 	cur = gc_lst->next;
// 	while(cur)
// 	{
// 		if (cur->data == ptr)
// 		{
// 			cur->ref_count--;
// 			if (cur->ref_count <= 0)
// 			{
// 				prev->next = cur->next;
// 				free(cur->data);
// 				free(cur);
// 			}
// 			return ;
// 		}
// 		prev = cur;
// 		cur = cur->next;
// 	}
// 	printf("can't found to decrease referenced counts\n");
// 	return ;
// }

# endif
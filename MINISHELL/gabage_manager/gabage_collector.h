//reference count :  number of times referenced
#include <stdio.h>

typedef struct s_gc_list
{
	void				*data;
	struct s_gc_list	*next;
	//int					ref_count;
}	t_gc_list;

//empty node, just a head node as dummy.
t_gc_list	*init_gc_list(void)
{
	t_gc_list *head = malloc(sizeof(t_gc_list));
	if (!head)
	{
		return (NULL);
	}
	head->data = NULL;
	head->next = NULL;
	//head->ref_count = 0;
	return head;
}

void	*do_alloc(t_gc_list *gc_lst, size_t howmuch)
{
	t_gc_list	*new_node = malloc(sizeof(t_gc_list));
	if (!new_node)
	{
		return (NULL);
	}
	void *data;
	data = malloc(howmuch);
	if (!data)
	{
		free (new_node);
		return NULL;
	}
	new_node->data = data;
	new_node->next = gc_lst->next;
	//new_node->ref_count = 1;
	gc_lst->next = new_node;
	return (data);
}

void	gc_free(t_gc_list **gc_lst)
{
	if (!gc_lst || !(*gc_lst))
		return ;

	t_gc_list	*cur;
	t_gc_list	*next;
	
	cur = (*gc_lst)->next;
	while(cur)
	{
		next = cur->next;
		free(cur->data);
		free(cur);
		cur = next;
	}
	free(*gc_lst);
    *gc_lst = NULL;
}

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

void	print_list(t_gc_list *gc_lst)
{
	t_gc_list	*cur;
	int			i;

	i = 0;
	cur = gc_lst->next;
	while(cur != NULL)
	{
		printf("[%d]th NODE, %p\n",i, cur->data);
		i++;
		cur = cur->next;
	}
}
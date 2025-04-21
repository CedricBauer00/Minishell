# include "garbage_collector.h"

t_gc_list	*init_gc_list(void)
{
	t_gc_list *head = malloc(sizeof(t_gc_list));
	if (!head)
	{
		return (NULL);
	}
	head->data = NULL;
	head->next = NULL;
	head->type = TYPE_SINGLE_CHAR;
	//head->ref_count = 0;
	return head;
}

void	*do_alloc(t_gc_list *gc_lst, size_t howmuch, t_data_type data_type)
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
	new_node->type = data_type;
	//new_node->ref_count = 1;
	gc_lst->next = new_node;
	return (data);
}

void	free_data_type(void *data, t_data_type data_type)
{
	if(!data)
		return ;
	if (data_type == TYPE_SINGLE_CHAR)
		free(data);
	else if (data_type == TYPE_DOUBLE_CHAR)
	{
		char ** temp = (char **)data;
		int i = 0;
		while(temp[i])
		{
			free(temp[i]);
			i++;
		}
		free (temp);
	}
}

void	gc_free(t_gc_list **gc_lst, t_data_type data_type)
{
	if (!gc_lst || !(*gc_lst))
		return ;

	t_gc_list	*cur;
	t_gc_list	*next;
	
	cur = (*gc_lst)->next;
	while(cur)
	{
		next = cur->next;
		printf("%p is free, type is %d\n", cur->data , cur->type);
		free_data_type(cur->data, cur->type);
		free(cur);
		cur = next;
	}
	printf("at last %p		is free\n", *gc_lst);
	free(*gc_lst);
    *gc_lst = NULL;
}

void	print_list(t_gc_list **gc_lst)
{
	t_gc_list	*cur;
	int			i;

	i = 1;
	cur = (*gc_lst)->next;
	while(cur != NULL)
	{
		printf("[%d]th NODE, %p\n",i, cur->data);
		i++;
		cur = cur->next;
	}
}

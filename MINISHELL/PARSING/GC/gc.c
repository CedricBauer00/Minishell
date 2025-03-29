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
	head->type = 0;
	//head->ref_count = 0;
	return head;
}

void	*do_alloc(t_gc_list *gc_lst, size_t howmuch, t_data_type data_type)
{
	if (!gc_lst)
		return NULL;
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
	if (data_type == TYPE_SINGLE_PTR)
	{
		if (data)
		{
			free(data);
			data = NULL;
		}
	}
	else if (data_type == TYPE_DOUBLE_PTR)
	{
		char **temp = (char **)data;
		// int i = 0;
		// while(temp[i])
		// {
		// 	free(temp[i]);
		// 	temp[i] = NULL;
		// 	i++;
		// }
		free (temp);
		temp = NULL;
	}
	//todo add case for struct
}

t_gc_list	*find_node(t_gc_list *gc_lst, void *target)
{
	t_gc_list	*cur;

	cur = gc_lst->next;
	while(cur)
	{
		if(cur->data == target)
		{
			printf("----------find : %p in garbage--------\n",target);
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

void	delete_node(t_gc_list **gc_lst, t_gc_list *to_delete)
{
	if (!gc_lst || !(*gc_lst))
		return ;

	t_gc_list	*cur;
	t_gc_list	*prev;

	prev = *gc_lst;
	cur = (*gc_lst)->next;
	while(cur)
	{
		if (cur == to_delete)
		{
			printf("delete node();%p is free, type is %d\n", cur->data , cur->type);
			free_data_type(cur->data, cur->type);
			prev ->next = cur ->next;
			free(cur);
			cur = NULL;
			break;
		}
		prev = cur;
		cur = cur -> next;
	}
}

void	all_free(t_gc_list **gc_lst)
{
	if (!gc_lst || !(*gc_lst) || !(*gc_lst)->next)
		return ;

	t_gc_list	*cur;
	t_gc_list	*next;
	
	cur = (*gc_lst)->next;
	next = NULL;
	while (cur)
    {
        next = cur->next;
        if (cur->data)
		{
			printf("all_free(); %p is free, type is %d, data: %s\n", cur->data , cur->type, (char*)cur->data);
            free_data_type(cur->data, cur->type);
			cur->data = NULL;
		}
		free(cur);
        cur = next;
    }
	free(*gc_lst);
    *gc_lst = NULL;
	
}

void	print_list(t_gc_list *gc_lst)
{
	t_gc_list	*cur;
	int			i;

	i = 1;
	cur = gc_lst->next;
	while(cur != NULL)
	{
		printf("[%d]th NODE[%s], %p\n",i, (char *)cur->data, cur->data);
		i++;
		cur = cur->next;
	}
}

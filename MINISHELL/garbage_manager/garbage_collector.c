# include "garbage_collector.h"

//memo when you initialize it use this!
t_gc	*get_gc(void)
{
	static t_gc *gc = {0};
	if (!gc)
	{
		gc = init_gc();
	}
	return (gc);
}

//memo when you initialize it use this!
// t_gc_list	*get_garbage_collector(void)
// {
// 	static t_gc_list *gc_list = {0};
// 	if (!gc_list)
// 	{
// 		gc_list = init_gc_list();
// 	}
// 	return (gc_list);
// }

t_gc	*init_gc(void)
{
	t_gc *gc = malloc(sizeof(t_gc));
	if (!gc)
	{
		return (NULL);
	}
	gc->temp = init_gc_list();
	if (!gc->temp)
		free(gc);
	gc->shell = init_gc_list();
	if (!gc->shell)
	{
		free(gc->temp);
		free(gc);
		return (NULL);
	}
	return gc;
}

t_gc_list	*init_gc_list(void)
{
	//this gonna be dummy node,which just indicate first node of list, it is nothing!
	t_gc_list *head = malloc(sizeof(t_gc_list));
	if (!head)
	{
		return (NULL);
	}
	head->data = NULL;
	head->next = NULL;
	head->type = 0;
	head->id = 0;
	//head->ref_count = 0;
	return head;
}

void	*do_alloc(t_gc_list **gc_lst, size_t howmuch, t_data_type data_type, char *id)
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
	new_node->next = NULL;
	new_node->type = data_type;
	new_node->id = id;
	new_node->level = (*gc_lst)->level;
	//new_node->ref_count = 1;
	if (*gc_lst)
		(*gc_lst)->next = new_node;
	return (data);
}

void	free_data_by_type(void *data, t_data_type data_type)
{
	if(!data)
		return ;
	if (data_type == TYPE_SINGLE_PTR)
	{
		free(data);
		data = NULL;
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

t_gc_list	*find_node_with_id(t_gc_list *gc_lst, char *id)
{
	t_gc_list	*cur;

	cur = gc_lst->next;
	while(cur)
	{
		//todo later we have to chnge ft_strncmp
		if(strcmp(cur->id, id) == 0)
		{
			printf("----------find : %s in garbage--------\n",id);
			return cur;
		}
		cur = cur->next;
	}
	return cur;
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
			printf("%p is free, type is %d\n", cur->data , cur->type);
			free_data_by_type(cur->data, cur->type);
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
	if (!gc_lst || !(*gc_lst))
		return ;

	t_gc_list	*cur;
	t_gc_list	*next;
	
	cur = (*gc_lst)->next;
	while(cur)
	{
		next = cur->next;
		printf("%p is free, type is %d\n", cur->data , cur->type);
		free_data_by_type(cur->data, cur->type);
		free(cur);
		cur = next;
	}
}

void	gc_level_up(t_gc_list *gc_list)
{
	gc_list->level++;
}

void	gc_free_by_level(t_gc_list *gc_list)
{
	t_gc_list *cur;
	t_gc_list *prev;

	prev = gc_list;
	cur = gc_list->next;
	while (cur)
	{
		if(cur->level == gc_list->level)
		{
			t_gc_list *to_free = cur;
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
	if (!gc || !gc->shell || !gc->temp)
		return;
	if(gc->temp)
	{
		all_free(&gc->temp);
		free(gc->temp);
		gc->temp = NULL;
	}
	if(gc->shell)
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
	while(cur != NULL)
	{
		printf("[%d]th NODE[%s], %p\n",i, cur->data, cur->data);
		i++;
		cur = cur->next;
	}
}
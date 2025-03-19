# include "../../INCLUDE/main.h"

int	get_envp_count(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
	{
		count++;
	}
	return (count);
}

char	**copy_envp(t_gc_list *gc_lst, char **envp)
{
	int		count;
	char	**my_envp;
	int		i;

	i = 0;
	if (!gc_lst)
	{
		printf(RED "copy_envp()edge case\n" DEFAULT);
		return (NULL);
	}
	count = get_envp_count(envp);
	my_envp = do_alloc(gc_lst, sizeof(char *) * count, TYPE_DOUBLE_CHAR);
	if (!my_envp)
	{
		printf(RED "copy_envp()edge case\n" DEFAULT);
		return (NULL);
	}
	printf("my_nevp %p\n", my_envp);
	while (envp[i])
	{
		my_envp[i] = strdup(envp[i]);
		if (!my_envp[i])
		{
			while(--i >= 0)
			{
				free(my_envp[i]);
			}
			return (NULL);
		}
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_gc_list *head = init_gc_list();
	
	char **test = copy_envp(head, envp);
	int i = 0;
	while (test[i])
	{
		printf(BLUE "test[%d], %s:\n" DEFAULT ,i, test[i]);
		i++;
	}
	printf(YELLOW "head node %p\n",head);
	print_list(head);

	printf("%p\n", head->data);
	printf("%p\n", head->next->data); DEFAULT;
	gc_free(&head, head->type);
}
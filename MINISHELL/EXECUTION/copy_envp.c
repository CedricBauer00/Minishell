# include "../INCLUDE/main.h"

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
	my_envp = (char**)do_alloc(&gc_lst, sizeof(char *) * (count + 1), TYPE_DOUBLE_PTR, "copy_envp");
	if (!my_envp)
	{
		printf(RED "copy_envp()edge case\n" DEFAULT);
		return (NULL);
	}
	while (i < count)
	{
		my_envp[i] = gc_strdup(envp[i], gc_lst);
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
	my_envp[count] = NULL;
	return (my_envp);
}

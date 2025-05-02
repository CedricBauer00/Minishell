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

// char	**copy_envp(t_gc_list *gc_lst, char **envp)
// {
// 	int		count;
// 	char	**my_envp;
// 	int		i;
	
// 	i = 0;
// 	if (!gc_lst)
// 		return (NULL);
// 	count = get_envp_count(envp);
// 	my_envp = (char**)do_alloc(&gc_lst, sizeof(char *) * (count + 1), \
// 		TYPE_DOUBLE_PTR, "copy_envp");
// 	if (!my_envp)
// 		return (NULL);
// 	while (i < count)
// 	{
// 		my_envp[i] = gc_strdup(envp[i], gc_lst);
// 		if (!my_envp[i])
// 			return (NULL);
// 		i++;
// 	}
// 	my_envp[count] = NULL;
// 	return (my_envp);
// }

char	**copy_envp(t_gc *gc, char **envp)
{
	int		count;
	char	**my_envp;
	int		i;

	i = 0;
	if (!gc)
		return (NULL);
	count = get_envp_count(envp);
	my_envp = (char**)do_alloc(&gc->shell, sizeof(char *) * (count + 1), \
		TYPE_DOUBLE_PTR, "copy_envp");
	if (!my_envp)
		return (gc_free(gc), NULL);
	while (envp[i])
	{
		my_envp[i] = gc_strdup(envp[i], &gc->shell);
		if (!my_envp[i])
			gc_free(gc);
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}
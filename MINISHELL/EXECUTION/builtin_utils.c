#include "../INCLUDE/main.h"

char	*my_getenv(char **my_envp, char *find, size_t find_len, t_gc_list *gc_lst)
{
	int	i;
	char *result;

	i = 0;
	while(my_envp[i])
	{
		if (strncmp(my_envp[i], find, find_len) == 0 && my_envp[i][find_len] == '=')
		{
			result = my_envp[i] + find_len + 1;
			return gc_strdup(result, gc_lst);
		}
		i++;
	}
	return NULL;
}
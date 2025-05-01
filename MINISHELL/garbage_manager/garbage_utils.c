#include "garbage_collector.h"

char	*gc_strdup(const char *str, t_gc_list *gc_lst)
{
	size_t	len;
	size_t	counter;
	char	*ptr;

	len = strlen(str);
	ptr = do_alloc(&gc_lst, len * sizeof(char) + 1,  TYPE_SINGLE_PTR, "gc_strdup");
	if (!ptr)
		return (NULL);
	counter = 0;
	while (str[counter] != '\0')
	{
		ptr[counter] = str[counter];
		counter++;
	}
	ptr[counter] = '\0';
	return (ptr);
}
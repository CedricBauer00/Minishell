#include "../INCLUDE/main.h"

void	is_exited(void *failed, t_gc_list *gc_lst)
{
	if (!failed)
	{
		perror(RED"making new_pipe in add_pipe()error\n"DEFAULT);
		all_free(gc_lst);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf(BLUE"alloc success\n"DEFAULT);
	}
}
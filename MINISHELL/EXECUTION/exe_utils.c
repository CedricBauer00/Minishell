#include "../INCLUDE/main.h"

void	is_exited(void *failed, t_gc_list *gc_lst)
{
	if (!failed)
	{
		perror(RED"in is_exited() making new_pipe in add_pipe()error\n"DEFAULT);
		gc_lst_free(&gc_lst);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf(BLUE"in is_exited() alloc success\n"DEFAULT);
	}
}
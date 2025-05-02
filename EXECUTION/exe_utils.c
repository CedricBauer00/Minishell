#include "../INCLUDE/main.h"

void	is_exited(void *failed, t_gc *gc)
{
	if (!failed)
	{
		perror(RED"in is_exited() making new_pipe in add_pipe()error\n"DEFAULT);
		gc_free(gc);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf(BLUE"in is_exited() alloc success\n"DEFAULT);
	}
}
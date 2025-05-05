#include "execution.h"

void	is_exited(void *failed, t_gc *gc)
{
	if (!failed)
	{
		printf(RED"%s in is_exited()is null \n"DEFAULT,(char*)failed);
		gc_free(gc);
		exit(EXIT_FAILURE);
	}
	else
		printf(BLUE"in is_exited() alloc success\n"DEFAULT);
}
#include "execution.h"

void	signal_handler_for_child(int sign)
{
	t_gc	*gc;

	gc = get_gc();
	if (sign == SIGINT) //ctrl +d
	{
		fprintf(stderr, RED"signal SIGINT heredoc\n"DEFAULT);
		gc_free(gc);
	}
	if (sign == SIGQUIT) // ctrl + backslash
	{
		fprintf(stderr, RED"signal SIGQUIT\n"DEFAULT);
		gc_free(gc);
		exit(1);
	}
}


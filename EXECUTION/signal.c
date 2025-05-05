#include "execution.h"

void	signal_handler_for_heredoc(int sign)
{
	t_gc	*gc;
	//t_shell	*shell;

	//shell = get_shell();
	gc = get_gc();
	if (sign == SIGINT)
	{
		//shell->heredoc_interrupted = 1;
		fprintf(stderr, RED"signal SIGINT heredoc\n"DEFAULT);
		// write(1, "\n", 1);
		// rl_redisplay();
	}
	if (sign == SIGQUIT)
	{
		fprintf(stderr, RED"signal SIGQUIT\n"DEFAULT);
		//exit(1);
	}
}


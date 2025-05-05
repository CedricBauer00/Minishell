#include "execution.h"

void	signal_handler()
{
	write(STDIN_FILENO, "i won't die!\n", 13);
}
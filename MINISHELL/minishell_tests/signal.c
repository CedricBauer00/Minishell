#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(void)
{
	char *input;

	signal(SIGINT, handle_sigint);  // Ctrl+C
	signal(SIGQUIT, SIG_IGN);       // Ctrl+\ wird ignoriert

	while (1)
	{
		input = readline("minishell> ");
		if (!input)  // Ctrl+D → readline gibt NULL zurück
		{
			printf("exit\n");
			break;
		}
		if (*input)  // wenn Eingabe nicht leer ist
			add_history(input);
		printf("Eingabe: %s\n", input);
		free(input);
	}
	return (0);
}
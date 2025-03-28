#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
	int	i = 0;
	char	*line = readline("minishell> \033[0m");
	
	while (line[i] != '\0')
	{
		if (line[i] == '|')
			printf("pipe_token\n");
		else if (line[i] == '\'')
			printf("single_quote\n");
		else if (line[i] == '\"')
			printf("dquote_token\n");
		else if (line[i] == '<')
			printf("redirect_in_token\n");
		else if (line[i] == '>')
			printf("redirect_out_token\n");
		else if (line[i] == '>' && line[i + 1] == '>')
			printf("append_token\n");
		else if (line[i] == '<' && line[i + 1] == '<')
			printf("heredoc_token\n");
		i++;
	}
	if (line && line[0] != '\0')
	{
		printf("\033[0m %s\n", line);
		free(line);
	}
	else if (line && line[0] == '\0')
	{
		printf("\033[33mYou presseed only Enter!\033[0m\n");
		free(line);
	}
	else
		printf("\033[31mYou doesn't entered anything!\033[0m\n");
	return (0);
}

// int main(void)
// {
// 	char *line;

// 	// while ((line = readline("minishell> ")) != NULL)
// 	while ((line = readline("minishell> ")) != NULL)
// 	{
// 		if (line && line[0] == '\0')
// 		{
// 			printf("\033[32mYou entered:\033[0m %s\n", line);
// 			free(line);
// 		}
// 		add_history(line);
// 		if (strcmp(line, "clear") == 0)
// 		{
// 			rl_clear_history();
// 			printf("History cleared!\n");
// 		}
// 		if (strcmp(line, "exit") == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		else
// 			printf("%s\n", line);
// 		}
// 	free(line);
// 	return (0);
// }


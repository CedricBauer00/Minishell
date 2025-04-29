#include "parsing.h"

void print_token(t_token *tokens)
{
	if (!tokens)
		return ;
	if (tokens->type & TOKEN_ARG)
		printf("Token Type: %s, Value: %s\n", "Arg", tokens->value);
	else if (tokens->type & TOKEN_BUILT_IN)
		printf("Token Type: %s, Value: %s\n", "Built-in", tokens->value);
	else if (tokens->type & TOKEN_PIPE)
		printf("Token Type: %s, Value: %s\n", "pipe", tokens->value);
	else if (tokens->type & TOKEN_REDIRECT_IN)
		printf("Token Type: %s, Value: %s\n", "redirect_in", tokens->value);
	else if (tokens->type & TOKEN_REDIRECT_OUT)
		printf("Token Type: %s, Value: %s\n", "redirect_out", tokens->value);
	else if (tokens->type & TOKEN_APPEND)
		printf("Token Type: %s, Value: %s\n", "append", tokens->value);
	else if (tokens->type & TOKEN_HEREDOC)
		printf("Token Type: %s, Value: %s\n", "heredoc", tokens->value);
	else if (tokens->type & TOKEN_VAR)
		printf("Token Type: %s, Value: %s\n", "VAR", tokens->value);
	else if (tokens->type & TOKEN_SPACES)
		printf("Token Type: %s, Value: %s\n", "SPACE", tokens->value);
	else if (tokens->type & TOKEN_FILE)
		printf("Token Type: %s, Value: %s\n", "FILE", tokens->value);
	else
		printf("Token Type: UNKNOWN (%d), Value: %s\n", tokens->type, tokens->value);
	// printf("Token Type: %d, Value: %s\n", tokens->type, tokens->value);
	tokens = tokens->next;
}

void	print_tokens(t_token *tokens)
{
    printf("\n---- Token List ----\n");
    t_token *current;

	current = tokens;

		while (current)
		{
			print_token(current);
			current = current->next;
		}
    printf("--------------------\n");
}

#include "parsing.h"


// void print_tokens(t_token *tokens)
// {
// 	printf("\n---- Token List ----\n");
// 	while (tokens)
// 	{
// 		if (tokens->type == 0)
// 			printf("Token Type: %s, Value: %s\n", "word", tokens->value);
// 		else if (tokens->type == 4)
// 			printf("Token Type: %s, Value: %s\n", "Built-in", tokens->value);
// 		else if (tokens->type == 5)
// 			printf("Token Type: %s, Value: %s\n", "pipe", tokens->value);
// 		else if (tokens->type == 6)
// 			printf("Token Type: %s, Value: %s\n", "redirect_in", tokens->value);
// 		else if (tokens->type == 7)
// 			printf("Token Type: %s, Value: %s\n", "redirect_out", tokens->value);
// 		else if (tokens->type == 8)
// 			printf("Token Type: %s, Value: %s\n", "append", tokens->value);
// 		else if (tokens->type == 9)
// 			printf("Token Type: %s, Value: %s\n", "heredoc", tokens->value);
// 		else if (tokens->type == 10)
// 			printf("Token Type: %s, Value: %s\n", "VAR", tokens->value);
// 		else
// 			printf("Token Type: UNKNOWN (%d), Value: %s\n", tokens->type, tokens->value);
// 		// printf("Token Type: %d, Value: %s\n", tokens->type, tokens->value);
// 		tokens = tokens->next;
// 	}
// 	printf("--------------------\n");
// }

void print_tokens(t_token *tokens)
{
    printf("\n---- Token List ----\n");
    if (!tokens)
		return (printf("\n----- No Tokens -----"));
	while (tokens->next)
        tokens = tokens->next;

        switch (tokens->type)
        {
            case 0:  printf("Token Type: word, Value: %s\n", tokens->value); break;
            case 4:  printf("Token Type: Built-in, Value: %s\n", tokens->value); break;
            case 5:  printf("Token Type: pipe, Value: %s\n", tokens->value); break;
            case 6:  printf("Token Type: redirect_in, Value: %s\n", tokens->value); break;
            case 7:  printf("Token Type: redirect_out, Value: %s\n", tokens->value); break;
            case 8:  printf("Token Type: append, Value: %s\n", tokens->value); break;
            case 9:  printf("Token Type: heredoc, Value: %s\n", tokens->value); break;
            case 10: printf("Token Type: VAR, Value: %s\n", tokens->value); break;
            default: printf("Token Type: UNKNOWN (%d), Value: %s\n", tokens->type, tokens->value); break;
        }
    printf("--------------------\n");
}

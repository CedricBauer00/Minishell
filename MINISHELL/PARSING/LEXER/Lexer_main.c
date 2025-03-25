/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/25 16:23:25 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCLUDE/parsing.h"

void	set_default(t_main *main)
{
	main->tokens = NULL;
	main->line = NULL;
	return ;
}

void	free_tokens(t_token *tokens)
{
	t_token *temp;
	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

int	append_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;
	
	if (!new_token)
		return (-1);// if new token is null //wie schreibe ich für diesen Fall eine freeing function?
	if (!*tokens)//if List os empty - set first token
	{
		*tokens = new_token;
		return (0);
	}
	// else append new token to list
	current = *tokens;
	while (current->next)
		current = current->next;//get last element of list
	// if (current)
	// 	printf("type = %d\n", current->type);
	// else
	// 	printf("Error: current is NULL\n");
	current->next = new_token;//appending to last next pinter
	return (0);
}

int	create_token(t_token **tokens, t_token_type type, char *str)
{
	t_token *new_token;
	
	if (!str)
		return (perror("ERROR\nStr is NULL!\n"), -1);//wie schreibe ich für diesen Fall eine freeing function?
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (perror("ERROR\nMalloc failed!\n"), -1);//wie schreibe ich für diesen Fall eine freeing function?
	new_token->type = type;
	new_token->value = ft_strdup(str);
	new_token->next = NULL;
	printf("Created Token: Type = %d, Value = %s\n", type, new_token->value);
	return (append_token(tokens, new_token));
}

void print_tokens(t_token *tokens)
{
	printf("\n---- Token List ----\n");
	while (tokens)
	{
		if (tokens->type == 0)
			printf("Token Type: %s, Value: %s\n", "word", tokens->value);
		else if (tokens->type == 5)
			printf("Token Type: %s, Value: %s\n", "pipe", tokens->value);
		else if (tokens->type == 6)
			printf("Token Type: %s, Value: %s\n", "redirect_in", tokens->value);
		else if (tokens->type == 7)
			printf("Token Type: %s, Value: %s\n", "redirect_out", tokens->value);
		else if (tokens->type == 8)
			printf("Token Type: %s, Value: %s\n", "append", tokens->value);
		else if (tokens->type == 9)
			printf("Token Type: %s, Value: %s\n", "heredoc", tokens->value);
		else if (tokens->type == 10)
			printf("Token Type: %s, Value: %s\n", "squote", tokens->value);
		else if (tokens->type == 11)
			printf("Token Type: %s, Value: %s\n", "dquote", tokens->value);
		else if (tokens->type == 12)
			printf("Token Type: %s, Value: %s\n", "VAR", tokens->value);
		else
			printf("Token Type: UNKNOWN (%d), Value: %s\n", tokens->type, tokens->value);
		// printf("Token Type: %d, Value: %s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
	printf("--------------------\n");
}

void	freecase(t_main *main)
{
	if (main->line)
	{
		free(main->line);
		main->line = NULL;
	}
	if (main->tokens)
	{
		free_tokens(main->tokens);
		main->tokens = NULL;
	}
}

int main()
{
	int	i;
	int	error;
	int	ws;
	int	len;
	char *word;
	t_main main;
	char *next_line;
	char *new;
	t_gc_list *head;
	
	head = init_gc_list();
	main.tokens = NULL;
	error = 0;
	i = 0;
	ws = 0;
	len = 0;
	set_default(&main);
	
	// while ([j] != NULL)
	// {
	i = 0;
	// while (line[i])
	while (1)
	{
		main.line = readline("minishell> ");
		printf(YELLOW"%s\n"DEFAULT, main.line);
		i = 0;
		if (!main.line)
		{
			printf("exit\n");
			return (freecase(&main), 0);
		}
		if (ft_strncmp(main.line, "", 1) == 0)
		{
			printf("enter\n");
			return (freecase(&main), 0);
		}
		add_history(main.line);
		while (main.line[i])
		{
			while (main.line[i] && ft_isspace(main.line[i]))
			{
				i++;
				//printf(RED"is in?%d\n"DEFAULT, i);
			}
			// if (isspace )
			if (main.line[i] == '|')
			{
				error = create_token(&main.tokens, TOKEN_PIPE, "|");
				i++;
			}
			else if (main.line[i] == '>' && main.line[i + 1] == '>')
			{
				printf(RED"is in?%d\n"DEFAULT, i);
				error = create_token(&main.tokens, TOKEN_APPEND, ">>");
				i += 2;
			}
			else if (main.line[i] == '<' && main.line[i + 1] == '<')
			{
				printf(GREEN"<< ,%s , %c\n"DEFAULT, main.line, main.line[i]);
				if (!main.line[i + 2] || ft_isspace(main.line[i + 2]))
					return(perror("ERROR\nHeredoc failed!\n"), freecase(&main), -1);
				error = create_token(&main.tokens, TOKEN_HEREDOC, "<<");
				i += 2;
			}
			else if (main.line[i] == '<')
			{
				printf(GREEN"<, %s, %c\n"DEFAULT, main.line, main.line[i]);
				printf("Found '<', creating token...\n");
				error = create_token(&main.tokens, TOKEN_REDIRECT_IN, "<");
				if (error < 0)
					printf("ERROR: Token creation failed for '<'\n");
				i++;
			}
			else if (main.line[i] == '>')
			{
				printf(GREEN">, %s, %c, i: %d\n"DEFAULT, main.line, main.line[i], i);
				error = create_token(&main.tokens, TOKEN_REDIRECT_OUT, ">");
				i++;
				//break;
			}
			else if (main.line[i] == '\'')
			{
				// error = create_token(&main.tokens, TOKEN_QUOTE, "\'");
				ws = i + 1;
				while (1)
				{
					while (main.line[i] && main.line[i] != '\'')
						i++;
					if (main.line[i] == '\'')
					{
						word = ft_strndup(main.line + ws, i - ws);
						error = create_token(&main.tokens, TOKEN_QUOTE, word);
						i++;
						free(word);
						break ;
					}
					next_line = readline("> ");
					if (!next_line)
						return (printf("ERROR\nFailed!\n"), freecase(&main), -1);
					new = main.line;
					main.line = ft_strjoin(main.line, next_line);
					free(new);
					free(next_line);
				}
				// 	if (error < 0)
				// 	{
				// 		free(word);
				// 		return (perror("ERROR\nTokenizing single quotes failed!\n"), -1);
				// 	}
				// }
			}
			else if (main.line[i] == '"')
				error = create_token(&main.tokens, TOKEN_DQOUTE, "\"");
			else if (main.line[i] && main.line[i + 1] && main.line[i + 2] &&
				ft_strncmp(main.line + i, "EOF", 3) == 0 && !ft_isalnum(main.line[i + 3]))
			{
				printf(RED"is in?%d\n"DEFAULT, i);
				error = create_token(&main.tokens, TOKEN_EOF, "EOF");
				i += 2;
			}
				//checking for alpanumeric words (commands, arguments)
			else if (valid_char(main.line[i]) || main.line[i] == '_')
			{
				ws = i;
				printf(GREEN"ws : %d\n"DEFAULT, ws);
				while (main.line[i] && (valid_char(main.line[i]) || main.line[i] =='_'))
					i++;
				printf(GREEN"len : %d\n"DEFAULT, i);
				len = i - ws;
				word = ft_strndup(main.line + ws, len);
				printf(GREEN"word = %s\n"DEFAULT, word);
				error = create_token(&main.tokens, TOKEN_WORD, word);
				free(word);
				//i--;
			}
			else if (main.line[i] == '$')
			{
				ws = i++;
				while (main.line[i] && (valid_char(main.line[i]) || main.line[i] == '_'))
					i++;
				len = i - ws;
				if (len > 1)
					word = ft_strndup(main.line + ws, len);
				else
					word = ft_strdup("$");
				error = create_token(&main.tokens, TOKEN_VAR, word);
				free(word);
				i--;
			}
			else
			{
				printf("Warning: Unrecognized character '%c' at position %d\n", main.line[i], i);
				i++;
			}
			// else if (main.line[i] == '$')
			// {
			// 	ws = i++;//possibly i = 0 first
			// 	while (ft_isalnum(main.line[i]) || main.line[i] == '_')
			// 		i++;
			// }
			if (error < 0)
				return (perror("ERROR:\nTokenizing failed!\n"), freecase(&main), -1);
			// if (main.start == NULL)
			printf(RED"i = %d\n"DEFAULT, i);
			//i++;
		}
		print_tokens(main.tokens);
		freecase(&main);
	}
	write(1, "\n", 1);
}
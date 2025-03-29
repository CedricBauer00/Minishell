/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MAIN.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/29 14:54:42 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/parsing.h" //change -- libft in INCLUDE && parsing.h
#include "parsing.h"


void	set_default(t_main *main)
{
	main->tokens = NULL;
	main->line = NULL;
	main->last_status_exit = 0;
	main->envp = NULL;
	main->next_line = NULL;
	main->new = NULL;
	main->word = NULL;
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
	{
		free(new_token);
		return (-1);// if new token is null //wie schreibe ich für diesen Fall eine freeing function?
	}
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

int	create_token(t_token **tokens, t_token_type type, char *str, t_gc_list *gc_list)
{
	t_token *new_token;
	
	if (!str)
		return (perror("ERROR\nStr is NULL!\n"), -1);
	new_token = (t_token *)do_alloc(gc_list, sizeof(t_token),TYPE_SINGLE_PTR);
	if (!new_token)
		return (perror("ERROR\nMalloc failed!\n"), -1);
	printf(GREEN"new_token :%p\n"DEFAULT, new_token);
	new_token->type = type;
	new_token->value = gc_strdup(str, gc_list);
	if (!new_token->value)
	{
		free(new_token);
		return (perror("ERROR\nMalloc for new_token->value failed!\n"), -1);
	}
	printf(GREEN"new_token->value :%p\n"DEFAULT, new_token->value);
	new_token->next = NULL;
	//printf("Created Token: Type = %d, Value = %s\n", type, new_token->value);
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

// void checkleak(void)
// {
// 	system(RED"leask "DEFAULT);
// }
// atexit(&checkleak);

int main(int argc, char **argv, char **envp)
{
	
	(void)argc;
	(void)argv;
	int	i;
	int	error;
	int	ws;
	int	len;
	t_main main;
	t_gc_list *gc_list;
	gc_list = init_gc_list();
	main.envp = copy_envp(gc_list, envp);
	
	// int k = 0;
	// while(main.envp[k])
	// {
	// 	printf("%s\n", main.envp[k]);
	// 	k++;
	// }
	
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
		if (main.line)
		{
			printf(RED"prev main.line is deleted\n"DEFAULT);
			free(main.line);
		}
		main.line = readline(YELLOW"minishell> "DEFAULT);
		printf(GREEN"main.line = %s\n"DEFAULT, main.line);
		printf(YELLOW"%s\n"DEFAULT, main.line);
		i = 0;
		if (!main.line) //error
		{
			printf(RED"exit\n"DEFAULT);
			if (gc_list)
				all_free(&gc_list);
			exit(1);
		}
		if (ft_strncmp(main.line, "", 1) == 0)
		{
			printf("enter\n");
			return (all_free(&gc_list), 0);
		}
		add_history(main.line);
		while (main.line[i])
		{
			printf(RED"main.line[i] %c\n"DEFAULT, main.line[i]);
			while (main.line[i] && ft_isspace(main.line[i]))
			{
				i++;
				printf("is space : i : %d\n",i);
			}
			if (main.line[i] == '|')
			{
				error = create_token(&main.tokens, TOKEN_PIPE, "|", gc_list);
				printf(RED"pipe i : %d\n, "DEFAULT, i);
				i++;
				printf(RED"pipe after increased i : %d\n, "DEFAULT, i);
			}
			else if (main.line[i] == '>' && main.line[i + 1] == '>')
			{
				// printf(RED"is in?%d\n"DEFAULT, i);
				if (main.line[i + 2] == '>')
					return (perror("ERROR\nsyntax error near unexpected token `>`"), all_free(&gc_list), -1);
				error = create_token(&main.tokens, TOKEN_APPEND, ">>", gc_list);
				i += 2;
			}
			else if (main.line[i] == '<' && main.line[i + 1] == '<') //fix
			{
				// printf(GREEN"<< ,%s , %c\n"DEFAULT, main.line, main.line[i]);
				if (!main.line[i + 2] || ft_isspace(main.line[i + 2]))
					return(perror("ERROR\nHeredoc failed!\n"), all_free(&gc_list), -1);
				error = create_token(&main.tokens, TOKEN_HEREDOC, "<<", gc_list);
				i += 2;
			}
			else if (main.line[i] == '<')
			{
				printf(GREEN"<, %s, %c\n"DEFAULT, main.line, main.line[i]);
				printf("Found '<', creating token...\n");
				error = create_token(&main.tokens, TOKEN_REDIRECT_IN, "<", gc_list);
				if (error < 0)
					printf("ERROR: Token creation failed for '<'\n");
				i++;
			}
			else if (main.line[i] == '>')
			{
				printf(GREEN">, %s, %c, i: %d\n"DEFAULT, main.line, main.line[i], i);
				error = create_token(&main.tokens, TOKEN_REDIRECT_OUT, ">", gc_list);
				i++;
				//break;
			}
			
			else if (main.line[i] == '\'') //error 
			{
				// error = create_token(&main.tokens, TOKEN_QUOTE, "\'");
				ws = i + 1;
				i++;
				while (1)
				{
					while (main.line[i] && main.line[i] != '\'')
						i++;
					if (main.line[i] == '\'')
					{
						main.word = gc_strndup(main.line + ws, i - ws, gc_list);
						if (!main.word)
						{
							return (perror("ERROR\nAllocating main.word failed!\n"), all_free(&gc_list), -1);
							exit(1);
						}
						error = create_token(&main.tokens, TOKEN_WORD, main.word, gc_list);
						if (error < 0)
						{
							return (perror("ERROR\nToken creation failed!\n"), all_free(&gc_list), -1);
							exit(1);
						}
						i++;
						t_gc_list *todelte = find_node(gc_list, main.word);
						printf(RED"main.word : %p\n"DEFAULT, main.word);
						delete_node(&gc_list, todelte);
						// free(main.word);
						// main.word = NULL;
						break ;
						printf(RED"enter\n"DEFAULT);
					}
					main.next_line = readline("> ");
					if (!main.next_line)
						return (perror("ERROR\nFailed!\n"), all_free(&gc_list), -1);
					
					main.new = gc_strjoin(main.line, main.next_line, gc_list);
					if (!main.new)
					{
						free(main.next_line);
						return (perror("ERROR\nAllocating main.new failed!\n"), all_free(&gc_list), -1);
					}
					// main.line = main.new;
					main.line = gc_strjoin(main.line, main.next_line, gc_list);
					// printf(RED" main.next_line :%s ,%p"DEFAULT,main.next_line,  main.next_line);
					free(main.next_line);
					main.next_line = NULL;
					
						// main.old_line = main.line;
					// free(main.old_line);
					// free(main.next_line);
				}
			}
			
			// else if (main.line[i] == '"')
			// 	error = create_token(&main.tokens, TOKEN_DQOUTE, "\"");
			// else if (main.line[i] && main.line[i + 1] && main.line[i + 2] &&
			// 	ft_strncmp(main.line + i, "EOF", 3) == 0 && !ft_isalnum(main.line[i + 3]))
			// {
			// 	printf(RED"is in?%d\n"DEFAULT, i);
			// 	error = create_token(&main.tokens, TOKEN_EOF, "EOF");
			// 	i += 2;
			// }
				//checking for alpanumeric main.words (commands, arguments)
			else if (ft_isalnum(main.line[i]) || main.line[i] == '_')
			{
				ws = i;
				printf(GREEN"ws : %d\n"DEFAULT, ws);
				while (main.line[i] && (ft_isalnum(main.line[i]) || main.line[i] =='_'))
					i++;
				printf(GREEN"valid_char() i : %d\n"DEFAULT, i);
				len = i - ws;
				printf(RED"len %d\n"DEFAULT, len);
				printf(RED"main.word :%p, main.line :%p\n"DEFAULT, main.word, main.line);
				main.word = gc_strndup(main.line + ws, len, gc_list);
				printf(GREEN"main.word %p\n"DEFAULT, main.word);
				if (ft_strncmp(main.word, "cd", 2) == 0)
				{
					write(1, "here!\n", 6);
					error = create_token(&main.tokens, TOKEN_BUILT_IN, main.word, gc_list);
				}
				else
				{
					printf(GREEN"word = %s\n"DEFAULT, main.word);
					error = create_token(&main.tokens, TOKEN_WORD, main.word, gc_list);
				}
				t_gc_list *find = find_node(gc_list, main.word);
				(void)find;
				delete_node(&gc_list, find);
				// free(main.word);
				//i--;
			}
			// else if (main.line[i] == '$')
			// {
			// 	ws = i++;
			// 	while (main.line[i] && (valid_char(main.line[i]) || main.line[i] == '_'))
			// 		i++;
			// 	len = i - ws;
			// 	if (len > 1)
			// 		main.word = gc_strndup(main.line + ws, len);
			// 	else
			// 		main.word = ft_strdup("$");
			// 	error = create_token(&main.tokens, TOKEN_VAR, main.word);
			// 	free(main.word);
			// 	i--;
			// }
			else
			{
				printf("Warning: Unrecognized character '%c' at position %d\n", main.line[i], i);
			}
			// else if (main.line[i] == '$')
			// {
			// 	ws = i++;//possibly i = 0 first
			// 	while (ft_isalnum(main.line[i]) || main.line[i] == '_')
			// 		i++;
			// }
			if (error < 0)
				return (perror("ERROR:\nTokenizing failed!\n"),all_free(&gc_list), -1);
			// if (main.start == NULL)
			// printf(RED"i = %d\n"DEFAULT, i);
			//i++;
		}
		print_tokens(main.tokens);
		//todo : delete later temp data and node
	}
	printf(BLUE"while loop out\n"DEFAULT);
	if (gc_list)
		all_free(&gc_list);
	write(1, "\n", 1);
}

//single quotes fertig implementatiion
//double quotes fix
//built in commands with now copied envp
// ... so far

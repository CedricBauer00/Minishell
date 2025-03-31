/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_back_up.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/31 14:42:43 by cbauer           ###   ########.fr       */
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
	main->error = 0;
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
		else if (tokens->type == 4)
			printf("Token Type: %s, Value: %s\n", "Built-in", tokens->value);
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
	int	ws;
	int	len;
	t_main main;
	t_gc_list *gc_list;
	gc_list = init_gc_list();
	main.envp = copy_envp(gc_list, envp);

	i = 0;
	ws = 0;
	len = 0;
	set_default(&main);
	while (1)
	{
		// if (main.line)
		// {
		// 	printf(RED"prev main.line is deleted %p \n"DEFAULT, main.line);
		// 	free(main.line);
		// }
		
		main.line = readline(YELLOW"minishell> "DEFAULT);
		printf(YELLOW"main.line %s ,%p\n"DEFAULT, main.line, main.line);
		i = 0;
		if (!main.line) //error
		{
			printf(RED"exit\n"DEFAULT);
			if (gc_list)
				all_free(&gc_list);
			exit(1);
		}
		if (main.line == NULL)
			fprintf(stderr, RED"is already free %p\n"DEFAULT, main.line);
		if (ft_strncmp(main.line, "", 1) == 0)
		{
			printf(RED"enter %p\n"DEFAULT, main.line);
			if (gc_list)
			{
				all_free(&gc_list);
				printf(RED"already\n"DEFAULT);
			}
			exit(1);
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
				main.error = create_token(&main.tokens, TOKEN_PIPE, "|", gc_list);
				printf(RED"pipe i : %d\n, "DEFAULT, i);
				i++;
				printf(RED"pipe after increased i : %d\n, "DEFAULT, i);
			}
			else if (main.line[i] == '>' && main.line[i + 1] == '>')
			{
				// printf(RED"is in?%d\n"DEFAULT, i);
				if (main.line[i + 2] == '>')
					return (perror("ERROR\nsyntax main.error near unexpected token `>`"), all_free(&gc_list), -1);
				main.error = create_token(&main.tokens, TOKEN_APPEND, ">>", gc_list);
				i += 2;
			}
			else if (main.line[i] == '<' && main.line[i + 1] == '<') //fix
			{
				// printf(GREEN"<< ,%s , %c\n"DEFAULT, main.line, main.line[i]);
				if (!main.line[i + 2] || ft_isspace(main.line[i + 2]))
					return(perror("ERROR\nHeredoc failed!\n"), all_free(&gc_list), -1);
				main.error = create_token(&main.tokens, TOKEN_HEREDOC, "<<", gc_list);
				i += 2;
			}
			else if (main.line[i] == '<')
			{
				printf(GREEN"<, %s, %c\n"DEFAULT, main.line, main.line[i]);
				printf("Found '<', creating token...\n");
				main.error = create_token(&main.tokens, TOKEN_REDIRECT_IN, "<", gc_list);
				if (main.error < 0)
					printf("ERROR: Token creation failed for '<'\n");
				i++;
			}
			else if (main.line[i] == '>')
			{
				printf(GREEN">, %s, %c, i: %d\n"DEFAULT, main.line, main.line[i], i);
				main.error = create_token(&main.tokens, TOKEN_REDIRECT_OUT, ">", gc_list);
				i++;
				//break;
			}
			else if (main.line[i] == '\'') //main.error 
			{
				// if (quotes(&main, ws, i, gc_list) < 0)
				// 	return (perror("ERROR\nQuotes failed!\n"), all_free(&gc_list), -1);
				// main.error = create_token(&main.tokens, TOKEN_QUOTE, "\'");
				ws = i + 1;
				i++;
				while (1)
				{
					while (main.line[i] && main.line[i] != '\'') //0x5
						i++;
					if (main.line[i] == '\'')
					{
						main.word = gc_strndup(main.line + ws, i - ws, gc_list);  //0x0001 + >> main.word = 0x0002 . 0x00005 + ws
						if (!main.word)
						{
							return (perror("ERROR\nAllocating main.word failed!\n"), all_free(&gc_list), -1);
							exit(1);
						}
						// if (is_built_in(&main) == 1)
						// 	main.error = create_token(&main.tokens, TOKEN_BUILT_IN, main.word, gc_list);
						// else
						
						if ((ft_strncmp(main.word, "cd", 2) == 0 && main.word[2] == '\0')\
						|| (ft_strncmp(main.word, "echo", 4) == 0 && main.word[4] == '\0') \
						|| (ft_strncmp(main.word, "export", 6) == 0 && main.word[6] == '\0') \
						|| (ft_strncmp(main.word, "unset", 5) == 0 && main.word[5] == '\0') \
						|| (ft_strncmp(main.word, "env", 3) == 0 && main.word[3] == '\0') \
						|| (ft_strncmp(main.word, "pwd", 3) == 0 && main.word[3] == '\0') \
						|| (ft_strncmp(main.word, "exit", 4) == 0 && main.word[4] == '\0') )
							main.error = create_token(&main.tokens, TOKEN_BUILT_IN, main.word, gc_list);
						else
							main.error = create_token(&main.tokens, TOKEN_WORD, main.word, gc_list);
						if (main.error < 0)
						{
							return (perror("ERROR\nToken creation failed!\n"), all_free(&gc_list), -1);
							exit(1);
						}
						i++;
						t_gc_list *todelte = find_node(gc_list, main.word);
						printf(RED"main.word : %p\n"DEFAULT, main.word);
						delete_node(&gc_list, todelte);
						break ;
						printf(RED"enter\n"DEFAULT);
					}
					main.next_line = readline("> ");
					if (!main.next_line)
						return (perror("ERROR\nFailed!\n"), all_free(&gc_list), -1);
					printf(YELLOW"before usig strjoin%p\n"DEFAULT, main.line);
					main.line = gc_strjoin(main.line, main.next_line, gc_list);  //0x0001 ,0x0002 >> main.line 0x0005
					printf("main.line : %p\n", main.line);
					//free(main.line);
					printf(YELLOW"after usig strjoin%p\n"DEFAULT, main.newline);
					if (main.next_line)
					{
						printf(BLUE"free next_line\n"DEFAULT);
						free(main.next_line);
						main.next_line = NULL;
					}
				}
			}
			// else if (main.line[i] == '"')
			// {
			// 	ws = i + 1;
			// 	i++;
			// 	while (1)
			// 	{
			// 		while (main.line[i] && main.line[i] != '"')
			// 			i++;
			// 		if (main.line[i] == '"')
			// 		{
			// 			main.word = gc_strndup(main.line + ws, i - ws, gc_list);
			// 			if (!main.word)
			// 			{
			// 				return (perror("ERROR\nAllocating main.word failed!\n"), all_free(&gc_list), -1);
			// 				exit(1);
			// 			}
			// 			if (is_built_in(&main) == 1)
			// 				main.error = create_token(&main.tokens, TOKEN_BUILT_IN, main.word, gc_list);
			// 			else
			// 				main.error = create_token(&main.tokens, TOKEN_WORD, main.word, gc_list);
			// 			if (main.error < 0)
			// 			{
			// 				return (perror("ERROR\nToken creation failed!\n"), all_free(&gc_list), -1);
			// 				exit(1);
			// 			}
			// 			i++;
			// 			t_gc_list *todelte = find_node(gc_list, main.word);
			// 			printf(RED"main.word : %p\n"DEFAULT, main.word);
			// 			delete_node(&gc_list, todelte);
			// 			break ;
			// 			printf(RED"enter\n"DEFAULT);
			// 		}
			// 		main.next_line = readline("> ");
			// 		if (!main.next_line)
			// 			return (perror("ERROR\nFailed!\n"), all_free(&gc_list), -1);
					
			// 		main.new = gc_strjoin(main.line, main.next_line, gc_list);
			// 		if (!main.new)
			// 		{
			// 			free(main.next_line);
			// 			return (perror("ERROR\nAllocating main.new failed!\n"), all_free(&gc_list), -1);
			// 		}
			// 		main.line = gc_strjoin(main.line, main.next_line, gc_list);
			// 		free(main.next_line);
			// 		main.next_line = NULL;
			// 	}
			// }
			///////
				// main.error = create_token(&main.tokens, TOKEN_DQOUTE, "\"", gc_list);
			else if (main.line[i] && main.line[i + 1] && main.line[i + 2] &&
				ft_strncmp(main.line + i, "EOF", 3) == 0 && !ft_isalnum(main.line[i + 3]))
			{
				printf(RED"is in?%d\n"DEFAULT, i);
				main.error = create_token(&main.tokens, TOKEN_EOF, "EOF", gc_list);
				i += 2;
			}
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
				if (is_built_in(&main) == 1)
					main.error = create_token(&main.tokens, TOKEN_BUILT_IN, main.word, gc_list);
				else
				{
					printf(GREEN"word = %s\n"DEFAULT, main.word);
					main.error = create_token(&main.tokens, TOKEN_WORD, main.word, gc_list);
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
			// 		main.word = gc_strndup(main.line + ws, len, gc_list);
			// 	else
			// 		main.word = ft_strdup("$");
			// 	main.error = create_token(&main.tokens, TOKEN_VAR, main.word, gc_list);
			// 	free(main.word);
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
			if (main.error < 0)
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

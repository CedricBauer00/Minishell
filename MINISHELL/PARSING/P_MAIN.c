/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_MAIN.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/02 11:39:16 by cbauer           ###   ########.fr       */
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
	main->temp_for_line = NULL;
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
	new_token->type = type;
	new_token->value = gc_strdup(str, gc_list);
	if (!new_token->value)
	{
		free(new_token);
		return (perror("ERROR\nMalloc for new_token->value failed!\n"), -1);
	}
	new_token->next = NULL;
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
		else if (tokens->type == 10)
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
		main.temp_for_line = readline(YELLOW"minishell> "DEFAULT);
		main.line = do_alloc(gc_list,ft_strlen(main.temp_for_line) + 1, TYPE_SINGLE_PTR);
		strcpy(main.line, main.temp_for_line);
		// printf(YELLOW"main.line %s ,%p , main.temp_for_line %p\n"DEFAULT, main.line, main.line,main.temp_for_line);
		free(main.temp_for_line);
		i = 0;
		if (!main.line) //error
		{
			// printf(RED"exit\n"DEFAULT);
			if (gc_list)
				all_free(&gc_list);
			exit(1);
		}
		if (main.line == NULL)
			fprintf(stderr, RED"is already free %p\n"DEFAULT, main.line);
		if (ft_strncmp(main.line, "", 1) == 0)
		{
			// printf(RED"enter %p\n"DEFAULT, main.line);
			if (gc_list)
			{
				all_free(&gc_list);
				// printf(RED"already\n"DEFAULT);
			}
			exit(0);
		}
		add_history(main.line);
		while (main.line[i])
		{
			while (main.line[i] && ft_isspace(main.line[i]))
				i++;
			if (main.line[i] == '|')
				i = pipes(&main, gc_list, i);
			else if (main.line[i] == '>' && main.line[i + 1] == '>')
			{
				i = append(&main, i, gc_list);
				if (i < 0)
					return (perror("ERROR\nsyntax main->error near unexpected token `>`"), all_free(&gc_list), -1);
			}
			else if (main.line[i] == '<' && main.line[i + 1] == '<') //fix
			{
				i = heredoc(&main , i, gc_list);
				if (i < 0)
					return(perror("ERROR\nHeredoc failed!\n"), all_free(&gc_list), -1);
			}
			else if (main.line[i] == '<')
				i = redirect_in(&main, i, gc_list);
			else if (main.line[i] == '>')
				i = redirect_out(&main, i, gc_list);
			else if (main.line[i] == '\'') //main.error 
			{	
				if (quotes(&main,&i, gc_list) < 0)
					return (perror("ERROR\nQuotes failed!\n"), all_free(&gc_list), -1);
			}
			else if (main.line[i] == '"')
			{
				if (dquotes(&main,&i, gc_list) < 0)
					return (perror("ERROR\nQuotes failed!\n"), all_free(&gc_list), -1);
			}
			else if (main.line[i] && main.line[i + 1] && main.line[i + 2] &&
				ft_strncmp(main.line + i, "EOF", 3) == 0 && !ft_isalnum(main.line[i + 3]))
			{
				main.error = create_token(&main.tokens, TOKEN_EOF, "EOF", gc_list);
				i += 2;
			}
				//checking for alpanumeric main.words (commands, arguments)
			else if (ft_isalnum(main.line[i]) || main.line[i] == '_')
				words(&main, &i, ws, gc_list);
			else if (main.line[i] == '$')
				// variables(&main, i, ws, len, gc_list);
			{
				expands(&main, &i, gc_list);
			}
			else
			{
				printf("Warning: Unrecognized character '%c' at position %d\n", main.line[i], i);
			}
		
			if (main.error < 0)
				return (perror("ERROR:\nTokenizing failed!\n"),all_free(&gc_list), -1);
			// if (main.start == NULL)
			// printf(RED"i = %d\n"DEFAULT, i);
			//i++;
		}
		print_tokens(main.tokens);
		//todo : delete later temp data and node
	}
	// printf(BLUE"while loop out\n"DEFAULT);
	if (gc_list)
		all_free(&gc_list);
	write(1, "\n", 1);
}

//single quotes fertig implementatiion
//double quotes fix
//built in commands with now copied envp
// ... so far

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/25 11:29:32 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCLUDE/parsing.h"

void	set_default(t_main *main)
{
	main->start = NULL;
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
		return (-1);// if new token is null
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
		return (perror("ERROR\nStr is NULL!\n"), -1);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (perror("ERROR\nMalloc failed!\n"), -1);//errormessage
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

int main()
{
	int	i;
	int	j;
	int	error;
	int	ws;
	int	len;
	char *word;
	t_main main;
	t_token *tokens;
	char *line;
	char *next_line;
	char *new;
	
	tokens = NULL;
	error = 0;
	j = 1;
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
		line = readline("minishell> ");
		i = 0;
		if (!line)
		{
			printf("exit\n");
			return (0);
		}
		if (ft_strncmp(line, "", 1) == 0)
		{
			printf("enter\n");
			return (0);
		}
		add_history(line);
		while (line[i])
		{
			while (line[i] && ft_isspace(line[i]))
				i++;
			// if (isspace )
			if (line[i] == '|')
			{
				error = create_token(&tokens, TOKEN_PIPE, "|");
				i++;
			}
			else if (line[i] == '>' && line[i + 1] == '>')
			{
				error = create_token(&tokens, TOKEN_APPEND, ">>");
				i += 2;
			}
			else if (line[i] == '<' && line[i + 1] == '<')
			{
				if (!line[i + 2] || ft_isspace(line[i + 2]))
					return(perror("ERROR\nHeredoc failed!\n"), -1);
				error = create_token(&tokens, TOKEN_HEREDOC, "<<");
				i += 2;
			}
			else if (line[i] == '<')
			{
				printf("Found '<', creating token...\n");
				error = create_token(&tokens, TOKEN_REDIRECT_IN, "<");
				if (error < 0)
					printf("ERROR: Token creation failed for '<'\n");
				i++;
			}
			else if (line[i] == '>')
			{
				error = create_token(&tokens, TOKEN_REDIRECT_OUT, ">");
				i++;
			}
			else if (line[i] == '\'')
			{
				// error = create_token(&tokens, TOKEN_QUOTE, "\'");
				ws = i + 1;
				while (1)
				{
					while (line[i] && line[i] != '\'')
						i++;
					if (line[i] == '\'')
					{
						word = ft_strndup(line + ws, i - ws);
						error = create_token(&tokens, TOKEN_QUOTE, word);
						i++;
						free(word);
						break ;
					}
					next_line = readline("> ");
					if (!next_line)
						return (printf("ERROR\nFailed!\n"));
					new = line;
					line = ft_strjoin(line, next_line);
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
			else if (line[i] == '"')
				error = create_token(&tokens, TOKEN_DQOUTE, "\"");
			else if (line[i] && line[i + 1] && line[i + 2] &&
				ft_strncmp(line + i, "EOF", 3) == 0 && !ft_isalnum(line[i + 3]))
			{
				error = create_token(&tokens, TOKEN_EOF, "EOF");
				i += 2;
			}
				//checking for alpanumeric words (commands, arguments)
			else if (ft_isalnum(line[i]) || line[i] == '_')
			{
				ws = i;
				while (line[i] && (ft_isalnum(line[i]) || line[i] =='_'))
					i++;
				len = i - ws;
				word = ft_strndup(line + ws, len);
				error = create_token(&tokens, TOKEN_WORD, word);
				free(word);
				i--;
			}
			else if (line[i] == '$')
			{
				ws = i++;
				while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
					i++;
				len = i - ws;
				if (len > 1)
					word = ft_strndup(line + ws, len);
				else
					word = ft_strdup("$");
				error = create_token(&tokens, TOKEN_VAR, word);
				free(word);
				i--;
			}
			else
			{
				printf("Warning: Unrecognized character '%c' at position %d\n", line[i], i);
				i++;
			}
			// else if (line[i] == '$')
			// {
			// 	ws = i++;//possibly i = 0 first
			// 	while (ft_isalnum(line[i]) || line[i] == '_')
			// 		i++;
			// }
			if (error < 0)
				return (perror("ERROR:\nTokenizing failed!\n"), (-1));
			// if (main.start == NULL)
			printf("i = %d\n", i);
			i++;
		}
		j++;
	print_tokens(tokens);
	free_tokens(tokens);
	tokens = NULL;
	if (line)
		free(line);
	}
	write(1, "\n", 1);
}
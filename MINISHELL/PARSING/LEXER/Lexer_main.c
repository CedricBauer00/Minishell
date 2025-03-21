/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/20 17:24:41 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCLUDE/parsing.h"


void	set_default(t_main *main)
{
	main->start = NULL;
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
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
	current->next = new_token;//appending to last next pinter
	return (0);
}

int	create_token(t_token **tokens, t_token_type type, char *str)
{
	t_token *new_token;
	
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (-1);//errormessage
	new_token->type = type;
	new_token->value = ft_strdup(str);
	new_token->next = NULL;
	return (append_token(tokens, new_token));
}

int main(int argc, char **argv)
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
	
	tokens = NULL;
	error = 0;
	j = 1;
	i = 0;
	ws = 0;
	len = 0;
	set_default(&main);
	if (argc > 1)
	{
		
		// while (argv[j] != NULL)
		// {
			i = 0;
			// while (line[i])
			line = readline("minishell> ");
			while (line[i])
			{
				while (line[i] && ft_isspace(line[i]))
					i++;
				// if (isspace )
				if (line[i] == '|')
					error = create_token(&tokens, TOKEN_PIPE, "|");
				else if (line[i] == '>' && line[i + 1] == '>')
				{
					error = create_token(&tokens, TOKEN_APPEND, ">>");
					i++;
				}
				else if (line[i] == '<' && line[i + 1] == '<')
				{
					error = create_token(&tokens, TOKEN_HEREDOC, "<<");
					i++;
				}
				else if (line[i] == '<')
					error = create_token(&tokens, TOKEN_REDIRECT_IN, "<");
				else if (line[i] == '>')
					error = create_token(&tokens, TOKEN_REDIRECT_OUT, ">");
				else if (line[i] == '\'')
					error = create_token(&tokens, TOKEN_QUOTE, "\'");
				else if (line[i] == '"')
					error = create_token(&tokens, TOKEN_DQOUTE, "\"");
				else if (line[i] && argv[j][i + 1] && argv[j][i + 2] &&
					ft_strncmp(argv[j] + i, "EOF", 3) == 0 && !ft_isalnum(argv[j][i + 3]))
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
					word = ft_strndup(argv[j] + ws, len);
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
						word = ft_strndup(argv[j] + ws, len);
					else
						word = ft_strdup("$");
					error = create_token(&tokens, TOKEN_VAR, word);
					free(word);
					i--;
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
					
				i++;
			}
			j++;
		}
		free_tokens(tokens);
		tokens = NULL;
	// }
	write(1, "\n", 1);
}

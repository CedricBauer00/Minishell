/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/18 12:02:35 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	set_default(t_main *main)
{
	main->start = NULL;
}

int	appen_token(t_token **tokens, t_token *new_token)
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

int	*create_token(t_token **tokens, t_token_type type, char *str)
{
	t_token *new_token;
	
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
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

	t_main main;
	t_token tokens;
	error = 0;
	j = 1;
	i = 0;
	set_default(&main);
	if (argc > 1)
	{
		while (argv[j] != NULL)
		{
			i = 0;
			while (argv[j][i])
			{
				if (argv[j][i] == '|')
					error = create_token(&tokens, TOKEN_PIPE, '|');
				else if (argv[j][i] == '<')
					error = create_token(&tokens, TOKEN_REDIRECT_IN, '<');
				else if (argv[j][i] == '>')
					error = create_token(&tokens, TOKEN_REDIRECT_OUT, '>');
				else if (argv[j][i] == '>' && argv[j][i + 1] == '>')
					error = create_token(&tokens, TOKEN_APPEND, '>>');
				else if (argv[j][i] == '<' && argv[j][i + 1] == '<')
					error = create_token(&tokens, TOKEN_HEREDOC, '<<');
				else if (argv[j][i] == '\'')
					error = create_token(&tokens, TOKEN_QUOTE, '\'');
				else if (argv[j][i] == '"')
					error = create_token(&tokens, TOKEN_DQOUTE,  '\"');
				if (argv[j][i] == 'E' && argv[j][i + 1] == 'O' && argv[j][i + 2] == 'F')
					error = create_token(&tokens, TOKEN_EOF, 'EOF');
				if (error < 0)
					return (perror("ERROR:\nTokenizing failed!\n"), (-1));
				if (main.start == NULL)
					
				i++;
			}
			j++;
		}
	}
	write(1, "\n", 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_MAIN.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/04 11:38:38 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (-1);
	}
	if (!*tokens)
	{
		*tokens = new_token;
		return (0);
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
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
	t_shell *shell;
	t_gc_list *gc_list;
	
	set_default(&main);
	gc_list = init_gc_list();
	main.envp = copy_envp(gc_list, envp);
	shell = get_shell(gc_list);
	i = 0;
	ws = 0;
	len = 0;
	while (1)
	{
		main.temp_for_line = readline(YELLOW"minishell> "DEFAULT);
		if(!main.temp_for_line)
			break;
		main.line = do_alloc(gc_list,ft_strlen(main.temp_for_line) +1, TYPE_SINGLE_PTR);
		ft_strlcpy(main.line, main.temp_for_line,ft_strlen(main.line));
		free(main.temp_for_line);
		i = 0;
		if (!main.line)
		{
			if (gc_list)
				all_free(&gc_list);
			exit(1);
		}
		if (main.line == NULL)
			fprintf(stderr, RED"is already free %p\n"DEFAULT, main.line);
		if (ft_strncmp(main.line, "", 1) == 0)
		{
			if (gc_list)
				all_free(&gc_list);
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
				printf("i = %d\n", i);
				if (i < 0)
					return(perror("ERROR\nHeredoc failed!\n"), all_free(&gc_list), -1);
			}
			else if (main.line[i] == '<')
				i = redirect_in(&main, i, gc_list);
			else if (main.line[i] == '>')
				i = redirect_out(&main, i, gc_list);
			else if (main.line[i] == '\'')
			{	
				if (quotes(&main,&i, gc_list) < 0)
					return (printf("ERROR\nQuotes failed!\n"), all_free(&gc_list), -1);
			}
			else if (main.line[i] == '"')
			{
				if (dquotes(&main,&i, gc_list) < 0)
					return (fprintf(stderr, BLUE"ERROR\nQuotes failed!\n"DEFAULT), all_free(&gc_list), -1);
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
			{
				if (expands(&main, &i, gc_list) < 0)
					return (perror("ERROR\nExpand failed!\n"), -1);
			}
			else
				printf("Warning: Unrecognized character '%c' at position %d\n", main.line[i], i);
			if (main.error < 0)
				return (perror("ERROR:\nTokenizing failed!\n"),all_free(&gc_list), -1);
		}
		print_tokens(main.tokens);
	}
	if (gc_list)
		all_free(&gc_list);
	write(1, "\n", 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_MAIN.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/14 16:13:26 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	create_token(t_token **tokens, t_token_type type, char *str, t_gc_list *gc_list)
{
	t_token *new_token;
	
	if (!str)
		return (perror("ERROR\nStr is NULL!\n"), -1);
	new_token = (t_token *)do_alloc(gc_list, sizeof(t_token),TYPE_SINGLE_PTR);
	printf("create allocated token\n");
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
int	main_helper(t_main *main, t_gc_list *gc_list)
{
	// t_token	*temp;
	main->temp_for_line = readline(YELLOW"minishell> "DEFAULT);
	// temp = main->tokens;
	// while (1)
	// {
	// 	if (temp == NULL)
	// 		break ;
	// 	printf("main.tokens = %s\n", temp->value);
	// 	temp = temp->next;
	// }
	if(!main->temp_for_line)
		return (-1);
	main->line = do_alloc(gc_list, ft_strlen(main->temp_for_line) + 1, TYPE_SINGLE_PTR);
	ft_strlcpy(main->line, main->temp_for_line,ft_strlen(main->line));
	free(main->temp_for_line);
	if (!main->line)
	{
		if (gc_list)
			all_free(&gc_list);
		exit(1);
	}
	if (ft_strncmp(main->line, "history -c", 10) == 0)
		clear_history();
	else
		add_history(main->line); 
	return (0);
}

int	check_operator2(t_main *main, int *i, t_gc_list *gc_list)
{
	if (main->line[*i] == '"')
	{
		if (dquotes(main,i, gc_list) < 0)
			return (fprintf(stderr, BLUE"ERROR\nQuotes failed!\n"DEFAULT), all_free(&gc_list), -1);
	}
	else if (main->line[*i] && main->line[*i + 1] && main->line[*i + 2] &&
		ft_strncmp(main->line + *i, "EOF", 3) == 0 && !ft_isalnum(main->line[*i + 3]))
	{
		main->error = create_token(&main->tokens, TOKEN_EOF, "EOF", gc_list);
		*i += 2;
	}
	else if (main->line[*i] == '$')
	{
		if (expands(main, i, 0, gc_list) < 0)
			return (perror("ERROR\nExpand failed!\n"), -1);
	}
	else if (!ft_isspace(main->line[*i]))
		words(main, i, 0, gc_list);
	else
		printf("Warning: Unrecognized character '%c' at position %d\n", main->line[*i], *i);
	return (0);
}

int	check_operator(t_main *main, int *i, t_gc_list *gc_list)
{
	if (ft_isspace(main->line[*i]))
		main->error = create_token(&main->tokens, TOKEN_SPACES, " ", gc_list);
	while (main->line[*i] && ft_isspace(main->line[*i]))
		(*i)++;
	if (main->line[*i] == '\0')
		return (0);
	if (main->line[*i] == '>' && main->line[*i + 1] == '>')
		*i += operator(main, 2, main->line[*i], gc_list);
	else if (main->line[*i] == '<' && main->line[*i + 1] == '<')
		*i += operator(main, 2, main->line[*i], gc_list);
	// else if (main->line[*i] == '<' && main->line[*i + 1] == '<')
	// 	*i = heredoc(main, *i, gc_list);
	else if (main->line[*i] == '<' || main->line[*i] == '>' || main->line[*i] == '|')
		*i += operator(main, 1, main->line[*i], gc_list);
	else if (main->line[*i] == '\'')
	{	
		if (quotes(main, i, gc_list) < 0)
			return (printf("ERROR\nQuotes failed!\n"), all_free(&gc_list), -1);
	}
	else if (check_operator2(main, i, gc_list) < 0)
		return (-1);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	int	i;
	int	len;
	t_main main;
	t_shell *shell;
	t_gc_list *gc_list;
	
	using_history();
	set_default(&main);
	gc_list = init_gc_list();
	main.envp = copy_envp(gc_list, envp);
	shell = get_shell(gc_list);
	len = 0;
	while (1)
	{
		main.tokens = NULL;
		if (main_helper(&main, gc_list) < 0)
		{
			printf("1\n");
			return (printf("exit\n"), all_free(&gc_list), 0);
		}
		if (check_quote(&main) < 0)
		{
			printf("2\n");
			continue ;
		}
		i = 0;
		while (main.line[i])
		{
			printf("3\n");
			if (check_operator(&main, &i, gc_list) < 0 || i < 0)
			{
				printf("6\n");
				return (printf("ERROR\nCheck_operator failed!\n"), all_free(&gc_list), -1);
			}
			// printf("[i] = %c\n", main.line[i]);
		}
		if (check_for_node_spaces(&main, main.tokens, gc_list) < 0)
		{
			printf("5\n");	
			return (printf("ERROR\nChecking nodes failed!\n"), all_free(&gc_list), -1);
		}
		print_tokens(main.tokens);
		if (validate_syntax(main.tokens) < 0)
		{
			perror(RED"syntax error"DEFAULT);
			//t_gc_list *find;
			// find = find_node(gc_list, main.tokens->value);
			// delete_node(&gc_list, find);
			null_gc_node_free(&gc_list);
			print_list(gc_list);
			//continue ;
		}
		
	}
	if (gc_list)
		all_free(&gc_list);
	write(1, "\n", 1);
}


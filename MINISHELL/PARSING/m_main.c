/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/16 15:38:55 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// void checkleak(void)
// {
// 	system(RED"leask "DEFAULT);
// }
// atexit(&checkleak);

int	main_helper(t_main *main, t_gc_list *gc_list)
{
	main->temp_for_line = readline(YELLOW"minishell> "DEFAULT);
	if (!main->temp_for_line)
	{
		// Entferne den Prompt und gib "exit" auf der gleichen Zeile aus
		write(1, "\033[1A\033[2K\r", 8); // Cursor eine Zeile nach oben, lösche die Zeile und setze den Cursor an den Anfang
		printf("exit\n");
		return (-1);
	}
	main->line = do_alloc(&gc_list, ft_strlen(main->temp_for_line) + 1, TYPE_SINGLE_PTR, "input");
	if (!main->line)
		return (-1);
	ft_strlcpy(main->line, main->temp_for_line,ft_strlen(main->line));
	free(main->temp_for_line);
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
			return (fprintf(stderr, BLUE"ERROR\nQuotes failed!\n"DEFAULT), -1);
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
	// else if (main->line[*i] == '<' && main->line[*i + 1] == '<')
	// 	*i += operator(main, 2, main->line[*i], gc_list);
	else if (main->line[*i] == '<' && main->line[*i + 1] == '<')
		*i = heredoc(main, *i, gc_list);
	else if (main->line[*i] == '<' || main->line[*i] == '>' || main->line[*i] == '|')
		*i += operator(main, 1, main->line[*i], gc_list);
	else if (main->line[*i] == '\'')
	{	
		if (quotes(main, i, gc_list) < 0)
			return (printf("ERROR\nQuotes failed!\n"), -1);
	}
	else if (check_operator2(main, i, gc_list) < 0)
		return (-1);
	return (0);
}

void	lex_tokens_correctly(t_token *tokens)
{
	int	cmd;
	
	cmd = 0;
	while (tokens)
	{
		if (tokens->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT)
			&& tokens->next != NULL)
			tokens->next->type = TOKEN_FILE;
		// if (cmd == 0 && !(tokens->type & (TOKEN_APPEND | TOKEN_HEREDOC
		// 	| TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT)))
	}
}



int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	int	i;
	int	len;
	t_main main;
	t_shell *shell;
	t_gc	*gc;
	
	using_history();
	set_default(&main);
	signal(SIGINT, signal_func);
	signal(SIGQUIT, SIG_IGN);
	gc = get_garbage_collector();
	main.envp = copy_envp(gc->shell, envp);
	shell = get_shell(gc->shell);
	len = 0;
	while (1)
	{
		main.tokens = NULL;
		if (main_helper(&main, gc->temp) < 0)
			return (gc_free(gc), 0);
		if (check_quote(&main) < 0)
			continue ;
		i = 0;
		while (main.line[i])
		{
			if (check_operator(&main, &i, gc->temp) < 0 || i < 0)
				return (printf("ERROR\nCheck_operator failed!\n"), gc_free(gc), -1);
		}
		if (main.tokens && check_for_node_spaces(&main, main.tokens, gc->temp) < 0)
			return (printf("ERROR\nChecking nodes failed!\n"), gc_free(gc), -1);
		print_tokens(main.tokens);
		lex_tokens_correctly(main.tokens);
		if (validate_syntax(main.tokens) < 0)
		{
			perror(RED"syntax error"DEFAULT);
			all_free(&gc->temp);
			continue ;
		}
		//heredoc exection
		//normal exection
	}
	if (gc)
		gc_free(gc);
	write(1, "\n", 1);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/18 14:53:47 by cbauer           ###   ########.fr       */
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
		return (printf("exit\n"), 1);
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
		*i = heredoc(main, *i, gc_list);
	else if (main->line[*i] == '<' || main->line[*i] == '>' || main->line[*i] == '|' || main->line[*i] == '&')
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

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	check_for_command(t_token *tokens)
{
	if (is_built_in(tokens->value) == 1)
		return (tokens->type = TOKEN_BUILT_IN, 1);
	return (tokens->type = TOKEN_ARG, 1);
}

void	lex_tokens_correctly(t_token *tokens)
{
	int	cmd; // ob cmd gefunden wurde

	cmd = 0;
	while (tokens)
	{
		if (tokens->next != NULL && tokens->type & (TOKEN_REDIRECT_IN
			| TOKEN_REDIRECT_OUT | TOKEN_APPEND | TOKEN_HEREDOC)
			&& tokens->next->type & (TOKEN_ARG | TOKEN_BUILT_IN | TOKEN_VAR))
			tokens->next->type = TOKEN_FILE;
		else if (cmd == 0 && (tokens->type == TOKEN_ARG
			|| tokens->type == TOKEN_BUILT_IN || tokens->type == TOKEN_VAR))
			cmd = check_for_command(tokens); //cmd found
		else if (cmd == 1 && (tokens->type == TOKEN_ARG
			|| tokens->type == TOKEN_BUILT_IN || tokens->type == TOKEN_VAR))
			tokens->type = TOKEN_ARG;
		else if (tokens->type == TOKEN_PIPE)
			cmd = 0;
		tokens = tokens->next;
	}
}

int	ttyattr(void)
{
	struct termios	temp;

	if (tcgetattr(0, &temp) < 0)
		return (-1);
	temp.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(0, TCSANOW, &temp) < 0)
		return (-1);
	return (0);
}

int	main_loop(t_main *main, int i, t_gc *gc)
{
	int	indic;
	int	ret;

	while (1)
	{
		main->tokens = NULL;
		ret = main_helper(main, gc->temp);
		if (ret == 1) //exit fix!
			return (0);
		else if (ret == -1)
			continue ;
		if (check_quote(main) < 0)
			continue ;
		i = 0;
		while (main->line[i])
		{
			if (check_operator(main, &i, gc->temp) < 0 || i < 0)
				return (printf("ERROR\nCheck_operator failed!\n"), gc_free(gc), -1);
		}
		if (main->tokens && check_for_node_spaces(main, main->tokens, gc->temp) < 0)
			return (printf("ERROR\nChecking nodes failed!\n"), gc_free(gc), -1);
		lex_tokens_correctly(main->tokens);
		indic = validate_syntax(main->tokens);
		if (indic == -1)
		{
			all_free(&gc->temp);
			continue ;
		}
		if (indic == -2)
		{
			printf(RED"bonus error!\n"DEFAULT);
			all_free(&gc->temp);
			continue ;
		}
		print_tokens(main->tokens);
		//heredoc exection
		//normal exection
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_main main;
	t_shell *shell;
	t_gc	*gc;
	
	// int	indic;
	// int i;

	using_history();
	set_default(&main);
	signal(SIGINT, signal_func);
	signal(SIGQUIT, SIG_IGN);
	if (ttyattr() < 0)
		return (printf("ERROR\nttyattr failed!\n"), -1);
	gc = get_garbage_collector();
	main.envp = copy_envp(gc->shell, envp);
	shell = get_shell(gc->shell);
	if (main_loop(&main, 0, gc) < 0)
		return (printf("ERROR\nMain_loop failed!\n"), -1);
	if (gc)
		gc_free(gc);
}

// return (printf("exit"),fflush(stdout), 1);

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/23 15:00:21 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	main_helper(t_main *main, t_gc_list *gc_list)
{
	main->temp_for_line = readline(YELLOW"minishell> "DEFAULT);
	if (!main->temp_for_line)
		return (printf("exit\n"), 1);
	main->line = do_alloc(&gc_list, ft_strlen(main->temp_for_line) + 1, \
		TYPE_SINGLE_PTR, "input");
	if (!main->line)
		return (-1);
	ft_strlcpy(main->line, main->temp_for_line, ft_strlen(main->line));
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
		if (dquotes(main, i, gc_list) < 0)
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
		printf("Warning: Unrecognized character '%c' at position %d\n", \
			main->line[*i], *i);
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
	else if (main->line[*i] == '<' || main->line[*i] == '>'
		|| main->line[*i] == '|' || main->line[*i] == '&')
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

int	main_loop_helper(t_main *main, int indic, t_gc *gc)
{
	if (main->tokens && check_for_node_spaces(main, main->tokens, \
		gc->temp) < 0)
	return (printf("ERROR\nChecking nodes failed!\n"), \
		gc_free(gc), -1);
	lex_tokens_correctly(main->tokens);
	indic = validate_syntax(main->tokens, gc->temp);
	// printf("status = %d\n", get_shell(gc->temp)->last_status_exit);
	if (indic == -1)
		return (all_free(&gc->temp), -1);
	if (indic == -2)
		return (printf(RED"bonus error!\n"DEFAULT), all_free(&gc->temp), -1);
	print_tokens(main->tokens);
	return (0);
}

int	main_loop(t_main *main, int i, t_gc *gc)
{
	int	ret;

	while (1)
	{
		main->tokens = NULL;
		ret = main_helper(main, gc->temp);
		if (ret == 1)
			return (0);
		else if (ret == -1)
			continue ;
		if (check_quote(main) < 0)
			continue ;
		i = 0;
		while (main->line[i])
		{
			if (check_operator(main, &i, gc->temp) < 0 || i < 0)
				return (printf("ERROR\nCheck_operator failed!\n"), \
					gc_free(gc), -1);
		}
		if (main_loop_helper(main, 0, gc) < 0)
			continue ;
	}
	return (0);
}

/*
	heredoc exection
	normal exection
	right before print_tokens
*/

int	num_len(int n)
{
	int	len;

	len = 0;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void	init_shllvl(char *str, int i)
{
	char	*temp;
	int		j;
	int		len;

	j = 0;
	len = 6;
	temp = ft_itoa(i);
	str[0] = 'S';
	str[1] = 'H';
	str[2] = 'L';
	str[3] = 'V';
	str[4] = 'L';
	str[5] = '=';
	while (temp[j] != '\0')
	{
		str[len] = temp[j];
		j++;
		len++;
	}
	str[len] = '\0';
	free(temp);
}

int	incrmnt_shllvl(t_main *main, t_gc *gc)
{
	int	i;
	int	lvl;

	i = 0;
	lvl = 0;
	while (main->envp[i] != NULL)
	{
		if (ft_strncmp(main->envp[i], "SHLVL=", 6) == 0)
		{
			lvl = ft_atoi(main->envp[i] + 6);
			lvl++;
			break ;
		}
		i++;
	}
	main->envp[i] = do_alloc(&gc->shell, 7 + num_len(lvl), TYPE_SINGLE_PTR, "shllvl");
	if (!main->envp[i])
		return (-1);
	init_shllvl(main->envp[i], lvl);
	printf("%s\n", main->envp[i]);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_main	main;
	t_shell	*shell;
	t_gc	*gc;

	(void)argc;
	(void)argv;
	using_history();
	set_default(&main);
	signal(SIGINT, signal_func);
	signal(SIGQUIT, SIG_IGN);
	if (ttyattr() < 0)
		return (printf("ERROR\nttyattr failed!\n"), -1);
	gc = get_gc();
	main.envp = copy_envp(gc->shell, envp);
	if (incrmnt_shllvl(&main, gc) < 0)
		return (-1);
	shell = get_shell(gc->shell);
	if (main_loop(&main, 0, gc) < 0)
		return (printf("ERROR\nMain_loop failed!\n"), -1);
	if (gc)
		gc_free(gc);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:35:00 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/05 15:13:35 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	variables(t_main *main, int i, int ws, int len, t_gc_list *gc_list)
{
	ws = i++;
	while (main->line[i] && (valid_char(main->line[i]) || main->line[i] == '_'))
		i++;
	len = i - ws;
	if (len > 1)
		main->word = gc_strndup(main->line + ws, len, gc_list);
	else
		main->word = ft_strdup("$");
	main->error = create_token(&main->tokens, TOKEN_VAR, main->word, gc_list);
	free(main->word);
}

int	main_helper(t_main *main, t_gc_list *gc_list)
{
	main->temp_for_line = readline(YELLOW"minishell> "DEFAULT);
	if(!main->temp_for_line)
		return (-1);
	write(1, "HERE!\n", 6);
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
	write(1, "return\n", 7);
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
		write(1, "call\n", 5);
		if (main_helper(&main, gc_list) < 0)
			return (printf("ERROR\nMain helper failed!\n"), -1);
		i = 0;
		while (main.line[i])
		{
			while (main.line[i] && ft_isspace(main.line[i]))
				i++;
			if (check_operator(&main, &i, gc_list) < 0)
				return (printf("ERROR\nCheck_operator failed!\n"), all_free(&gc_list), -1);
			if (main.line[i] == '>' && main.line[i + 1] == '>')
			{
				i += operator(&main, 2, main.line[i], gc_list);
				if (i < 0)
					return (perror("ERROR\nsyntax main->error near unexpected token `>`"), all_free(&gc_list), -1);
			}
			else if (main.line[i] == '<' && main.line[i + 1] == '<') //fix
			{
				i = heredoc(&main , i, gc_list);
				if (i < 0)
					return(perror("ERROR\nHeredoc failed!\n"), all_free(&gc_list), -1);
			}
			else if (main.line[i] == '<' || main.line[i] == '>' || main.line[i] == '|')
				i += operator(&main, 1, main.line[i], gc_list);
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
				printf(RED"main.line[i] : %c, main.line[i + 1] : %c, main.line[i + 2] : %c\n"DEFAULT, main.line[i], main.line[i + 1], main.line[i + 2]);
				main.error = create_token(&main.tokens, TOKEN_EOF, "EOF", gc_list);
				i += 2;
			}
			else if (ft_isalnum(main.line[i]) || main.line[i] == '_' || main.line[i] == '-')
				words(&main, &i, 0, gc_list);
			else if (main.line[i] == '$')
			{
				if (expands(&main, &i, 0, gc_list) < 0)
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/06 16:46:39 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// void	set_default(t_main *main)
// {
// 	main->tokens = NULL;
// 	main->line = NULL;
// 	main->last_status_exit = 0;
// 	main->next_line = NULL;
// 	main->new = NULL;
// 	main->word = NULL;
// 	main->temp_for_line = NULL;
// 	main->error = 0;
// 	return ;
// }

t_main	*init_main_struct(t_gc_list **gc_lst)
{
	t_main	*main;

	main = do_alloc(gc_lst, sizeof(t_main), TYPE_SINGLE_PTR, "main");
	if (!main)
		return (NULL);
	main->tokens = NULL;
	main->line = NULL;
	main->last_status_exit = 0;
	main->next_line = NULL;
	main->new = NULL;
	main->word = NULL;
	main->temp_for_line = NULL;
	main->error = 0;
	return (main);
}

t_main	*get_main(void)
{
	static t_main *main = NULL; // Global static t_main struct
	t_gc	*gc;

	gc = get_gc();
	if (main == NULL)
	{
		main = init_main_struct(&gc->shell);
	}
	return (main);
}

// t_main *get_main_struct(void)
// {
// 	return &main;
// }

int	main_helper(t_main *main, t_gc_list **gc_temp)
{
	size_t len;

	main->temp_for_line = readline(YELLOW"minishell> "DEFAULT);
	len = ft_strlen(main->temp_for_line);
	if (!main->temp_for_line)
	{
		return (printf("exit\n"), 1);
	}
	main->line = do_alloc(gc_temp, len + 1, TYPE_SINGLE_PTR, "input");
	if (!main->line)
		return (-1);
	ft_strlcpy(main->line, main->temp_for_line, len + 1);
	if (!main->line)
		return (-1);
	free(main->temp_for_line);
	if (ft_strncmp(main->line, "history -c", 10) == 0)
		clear_history();
	else
		add_history(main->line);
	return (0);
}

int	main_loop_helper(t_main *main, int indic, t_gc *gc)
{
	if (main->tokens && check_for_node_spaces(main, main->tokens, \
		&gc->temp) < 0)
		return (printf("ERROR\nChecking nodes failed!\n"), \
		gc_free(gc), -1);
	lex_tokens_correctly(main->tokens);
	t_cmd_block *cmd_block = NULL;
	indic = validate_syntax(main->tokens);
	/*
	printf("status = %d\n", get_shell(gc->temp)->last_status_exit);
	*/
	if (indic == -1)
		return (all_free(&gc->temp), -1);
	if (indic == -2)
		return (printf(RED"bonus error!\n"DEFAULT), all_free(&gc->temp), -1);
	print_tokens(main->tokens);
	grouplize(main->tokens, &cmd_block, gc);
	main_execute(cmd_block);
	return (0);
}

int	main_loop(t_main *main, int i, t_gc *gc)
{
	int	ret;

	while (1)
	{
		main->tokens = NULL;
		ret = main_helper(main, &gc->temp);
		if (ret == 1)
			return (0);
		else if (ret == -1)
			continue ;
		if (check_quote(main) < 0)
			continue ;
		i = 0;
		while (main->line[i])
		{
			if (check_operator(main, &i, &gc->temp) < 0 || i < 0)
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

int	main(int argc, char **argv, char **envp)
{
	t_main	main;
	t_shell	*shell;
	t_gc	*gc;

	(void)argc;
	(void)argv;
	
	using_history();
	// set_default(&main);
	main = *get_main();
	signal(SIGINT, signal_func);
	signal(SIGQUIT, SIG_IGN);
	if (ttyattr() < 0)
		return (printf("ERROR\nttyattr failed!\n"), -1);
	gc = get_gc();
	shell = get_shell();
	shell->my_envp = copy_envp(gc, envp);
	if (!shell->my_envp)
		return (printf("ERROR\nCopy_envp failed!\n"), -1);
	if (incrmnt_shllvl(shell, gc) < 0)
		return (-1);
	// shell = get_shell(&gc->shell);
	if (main_loop(&main, 0, gc) < 0)
		return (printf("ERROR\nMain_loop failed!\n"), -1);
	if (gc)
	{
		printf(YELLOW"gc free execute!\n"DEFAULT);
		gc_free(gc);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/17 12:50:59 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	main_helper(t_main *main, t_gc_list **gc_temp)
{
	size_t	len;

	if (isatty(fileno(stdin)))
		main->temp_for_line = readline("minishell> ");
	len = 0;
	if (!main->temp_for_line)
		return (printf("exit\n"), 1);
	len = ft_strlen(main->temp_for_line);
	main->line = do_alloc(gc_temp, len + 1, TYPE_SINGLE_PTR, "input");
	if (!main->line)
		return (-1);
	ft_strlcpy(main->line, main->temp_for_line, len + 1);
	if (!main->line)
		return (-1);
	if (main->temp_for_line)
	{
		free(main->temp_for_line);
		main->temp_for_line = NULL;
	}
	if (ft_strncmp(main->line, "history -c", 10) == 0)
		clear_history();
	else
		add_history(main->line);
	return (0);
}

int	main_loop_helper(t_main *main, int indic, t_gc *gc, t_shell *shell)
{
	t_cmd_block	*cmd_block;
	t_cmd_block	*executable;

	cmd_block = NULL;
	if (main->tokens && check_for_node_spaces(main, main->tokens, \
		&gc->temp) < 0)
		return (gc_free(gc), -1);
	lex_tokens_correctly(main->tokens);
	indic = validate_syntax(main->tokens, 0, shell);
	if (indic == -1)
		return (all_free(&gc->temp), -1);
	shell->last_status_exit = 0;
	executable = grouplize(main->tokens, &cmd_block, gc);
	main_execute(executable);
	check_open_fds();
	if (gc->temp)
		all_free(&gc->temp);
	return (0);
}

int	main_loop(t_main *main, int i, t_gc *gc, t_shell *shell)
{
	int	ret;

	while (1)
	{
		i = 0;
		signal(SIGINT, signal_func);
		signal(SIGQUIT, SIG_IGN);
		main->tokens = NULL;
		ret = main_helper(main, &gc->temp);
		if (ret == 1)
			return (0);
		else if (ret == -1)
			continue ;
		if (check_quote(main) < 0)
			continue ;
		while (main->line[i])
		{
			if (check_operator(main, &i, &gc->temp) < 0 || i < 0)
				return (gc_free(gc), -1);
		}
		main_loop_helper(main, 0, gc, shell);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_main	main;
	t_shell	*shell;
	t_gc	*gc;

	using_history();
	set_default(&main, argc, argv);
	if (ttyattr() < 0)
		return (-1);
	gc = get_gc();
	shell = get_shell();
	if (envp && *envp)
		shell->my_envp = copy_envp(gc, envp, shell);
	else
		handle_no_env_minishell(shell, gc);
	if (!shell->my_envp)
		return (-1);
	if (incrmnt_shllvl(shell, gc) < 0)
		return (-1);
	if (main_loop(&main, 0, gc, shell) < 0)
		return (-1);
	if (gc)
		gc_free(gc);
}

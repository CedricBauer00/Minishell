/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_HEREDOC->c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:54:17 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/04 11:56:48 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// char	*heredoc_input(char *del, t_gc_list *gc_list)
// {
// 	char	*str;
// 	char	*input;
// 	int		iter;

// 	iter = 0;
// 	input = NULL;
// 	if (gc_list)
// 	{
// 		while (1)
// 		{
// 			str = readline("> ");
// 			if (!str)
// 				return (input);
// 			if (str && ft_strlen(str) == ft_strlen(del) && ft_strncmp(str, del, ft_strlen(del)) == 0)
// 			{
// 				printf("str = %s , %zu\n", str, ft_strlen(str));
// 				printf("del = %s, %zu\n", del, ft_strlen(del));
// 				printf("len = %zu\n", ft_strlen(str));
// 				return (printf("del found!\n"), free(str), input);
// 			}
// 			if (iter == 0)
// 				input = gc_strdup(str, gc_list); 
// 			else
// 			{
// 				input = gc_strjoin(input, "\n", gc_list);
// 				input = gc_strjoin(input, str, gc_list);
// 			}
// 			iter++;
// 		}
// 	}
// 	return (NULL);
// }

// int	heredoc_exec(char *input, t_main *main) // child prozess cs of cntrl c
// {
// 	pid_t pid;

// 	printf(RED"input = %s\n"DEFAULT, input);
// 	pid = fork();
// 	if(pid == 0)
// 	{
// 		fprintf(stderr,"is in?\n");
// 		execve("./minishell", NULL , main->envp);
// 	}
// 	return 1;
// }

// int	heredoc(t_main *main, int i, t_gc_list *gc_list) //why exit when press enter
// {
// 	int		j;
// 	int		ws;
// 	char	*del;
// 	char	*input;

// 	if (main->line[i + 2] == '\0')
// 		return (-1);
// 	j = i + 2;
// 	while (main->line[j] && check_char(main, j, 0) == 0)
// 		j++;
// 	ws = j;
// 	j = is_quote(main, j);
// 	j++; //add!!
// 	del = gc_strndup(&main->line[j], ws - j, gc_list);
// 	while (main->line[j] && !ft_isspace(main->line[j]))
// 		j++;
// 	input = heredoc_input( del, gc_list);
// 	if (!input)
// 		return (printf("ERROR\nHeredoc_exec failed!\n"), -1);
// 	// if (ft_strncmp(input, "minishell", 9) == 0)
// 	// {
// 	// 	if (heredoc_exec(input, main) < 0)
// 	// 		return (-1);
// 	// }
// 	if (main->line[i + 2] != '\0')
// 		main->error = create_token(&main->tokens, TOKEN_HEREDOC, "<<", gc_list);
// 	return (i += 2);
// }


// char	*heredoc_input(char *del, t_gc_list *gc_list)
// {
// 	char	*str;
// 	char	*input;
// 	int		iter;
// 	char *copied;

// 	iter = 0;
// 	input = NULL;
// 	if (gc_list)
// 	{
// 		while (1)
// 		{
// 			str = readline("> ");
// 			if (!str)
// 				return (input);
// 			if (str && ft_strncmp(str, del, ft_strlen(str)) == 0)
// 			{
// 				printf("str = %s , %zu\n", str, ft_strlen(str));
// 				printf("del = %s, %zu\n", del, ft_strlen(del));
// 				printf("len = %zu\n", ft_strlen(str));
// 				return (printf("del found!\n"), free(str), input);
// 			}
// 			if (ft_strncmp(str, del, ft_strlen(del)) == 0 && ft_strlen(str) == ft_strlen(del))
// 			{
// 				free(str);
// 				break ;
// 			}
// 			copied = gc_strdup(str, gc_list);
// 			if (iter == 0)
// 				input = copied; 
// 			else
// 			{
// 				input = gc_strjoin(input, "\n", gc_list);
// 				input = gc_strjoin(input, copied, gc_list);
// 			}
// 			iter++;
// 			free(str);
// 		}
// 	}
// 	return (input);
// }

//latest one

// int	heredoc(t_main *main, int i, t_gc_list *gc_list) //why exit when press enter
// {
// 	int		j;
// 	int		end;
// 	char	*word;

// 	// printf("i = %d\n", i);
// 	if (main->line[i + 2] == '\0')
// 		return (-1);
// 	j = i + 2;
// 	while (main->line[j] && check_char(main, j, 0) == 1)
// 		j++;
// 	// printf("j = %d\n", j);
// 	end = is_quote(main, j);
// 	word = gc_strndup(&main->line[j], end - j, gc_list);
// 	if (!word)
// 		return (-1);
// 	main->error = create_token(&main->tokens, TOKEN_HEREDOC, word, gc_list);
// 	return (end);
// }
// //remove quotes for heredoc execution 

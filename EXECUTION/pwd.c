/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:22 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 10:02:06 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*my_getcwd()
{
	char	*cwd;
	char	*temp;
	t_gc	*gc;

	gc = get_gc();
	temp = getcwd(NULL, 0);
	if (!temp)
	{
		perror(RED"getcwd()failed"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	cwd = (char*)do_alloc(&gc->temp, ft_strlen(temp) + 1, TYPE_SINGLE_PTR, "getcwd");
	if (!cwd)
	{
		gc_free(gc);
		return NULL; 
	}
	ft_strlcpy(cwd, temp, ft_strlen(temp) + 1);
	free(temp);
	return cwd;
}

void	ft_pwd(char **args, t_gc *gc)
{
	t_gc_list	*find;

	find = NULL;
	if (*args)
	{
		printf(RED"PWD TOO MANY ARGS\n"DEFAULT);
		return ;
	}
	char *pwd;
	pwd = my_getcwd();
	printf(YELLOW"%s\n"DEFAULT, pwd);
	find = find_node(find, (char*)pwd);
	delete_node(&gc->temp, (t_gc_list*)find);
}

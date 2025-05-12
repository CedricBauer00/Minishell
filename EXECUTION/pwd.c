/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:22 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/12 15:17:43 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*my_getcwd(void)
{
	char	*cwd;
	char	*temp;
	t_gc	*gc;

	gc = get_gc();
	temp = getcwd(NULL, 0);
	if (!temp)
	{
		gc_free(gc);
		exit(1);
	}
	cwd = (char *)do_alloc(&gc->temp, ft_strlen(temp) + 1, \
		TYPE_SINGLE_PTR, "getcwd");
	if (!cwd)
	{
		gc_free(gc);
		exit(1);
	}
	ft_strlcpy(cwd, temp, ft_strlen(temp) + 1);
	free(temp);
	return (cwd);
}

void	ft_pwd(char **args, t_gc *gc)
{
	t_gc_list	*find;
	char		*pwd;

	find = NULL;
	if (*args)
	{
		printf(RED"PWD TOO MANY ARGS\n"DEFAULT);
		return ;
	}
	pwd = my_getcwd();
	printf(YELLOW"%s\n"DEFAULT, pwd);
	find = find_node(find, (char *)pwd);
	delete_node(&gc->temp, (t_gc_list *)find);
}

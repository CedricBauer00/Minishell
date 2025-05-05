/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:04 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/05 15:07:04 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

//todo make export.
// char	**expand_envp(t_shell *shell, char *new_path)
// char *create_new_path(const char *name, const char *value)

//export even  if no existed '=' it should works
//if i use only with '=' we can see that var in env
void	print_envp(t_shell *shell)
{
	int	i;

	i = 0;
	if (!shell)
		return ;

	while(shell->my_envp[i])
	{
		if (extract_name(shell->my_envp[i]) && extract_value(shell->my_envp[i]) == NULL)
			printf(YELLOW"declare -x %s\n"DEFAULT, extract_name(shell->my_envp[i]));
		else
			printf(YELLOW"declare -x %s=\"%s\"\n"DEFAULT, extract_name(shell->my_envp[i]), extract_value(shell->my_envp[i]));
		i++;
	}
}



//todo fix must dont get digits.
void	export(char **args, t_shell *shell)
{
	t_gc	*gc;
	int		i;
	//bool	check;

	i = 0;
	gc = get_gc();
	if (args && args[0] == NULL)
	{
		print_envp(shell);
	}
	while ((args[i]))
	{
		//memo check is_valid_identifier
		// check = is_valid_identifier(args[i]);
		// if (check == false)
		// {
		// 	perror(RED"non valid identifier\n"DEFAULT);
		// 	return ;
		// 	//all_free(&gc->temp);
		// }
		char	*name = extract_name(args[i]);
		is_exited(name, gc);
		printf("name:%s\n", name);
		char	*value = extract_value(args[i]);
		printf("value:%s\n", value);
		ft_setenv(name, value, 1 , shell);
		if(name)
			free(name);
		if(value)
			free(value);
		i++;
	}
}

char	*extract_name(char *arg)
{
	int	i;

	i = 0;
	while(arg[i])
	{
		if (arg[i] == '=')
			break;
		i++;
	}
	return ft_substr(arg, 0, i);
}

char	*extract_value(char *arg)
{
	int	i;

	i = 0;
	while(arg[i] != '\0' && arg[i] != '=')
	{
		i++;
	}
	if (arg[i] == '=')
		return ft_strdup(arg + i + 1);
	else
		return NULL;
}

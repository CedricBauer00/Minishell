/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_copy_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:56:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/02 16:16:42 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// int	get_envp_count(char **envp)
// {
// 	int	count;

// 	count = 0;
// 	while (envp[count])
// 	{
// 		count++;
// 	}
// 	return (count);
// }

// char	**copy_envp(t_gc *gc, char **envp)
// {
// 	int		count;
// 	char	**my_envp;
// 	int		i;

// 	i = 0;
// 	if (!gc)
// 		return (NULL);
// 	count = get_envp_count(envp);
// 	my_envp = (char**)do_alloc(&gc->shell, sizeof(char *) * (count + 1), \
// 		TYPE_DOUBLE_PTR, "copy_envp");
// 	if (!my_envp)
// 		return (gc_free(gc), NULL);
// 	while (envp[i])
// 	{
// 		my_envp[i] = gc_strdup(envp[i], &gc->shell);
// 		if (!my_envp[i])
// 			gc_free(gc);
// 		i++;
// 	}
// 	my_envp[i] = NULL;
// 	return (my_envp);
// }

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
	main->envp[i] = do_alloc(&gc->shell, 7 + num_len(lvl), \
		TYPE_SINGLE_PTR, "shllvl");
	if (!main->envp[i])
		return (-1);
	init_shllvl(main->envp[i], lvl);
	printf("%s\n", main->envp[i]);
	return (0);
}

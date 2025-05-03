/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_copy_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:56:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/03 16:47:01 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

int	incrmnt_shllvl(t_shell *shell, t_gc *gc)
{
	int	i;
	int	lvl;

	i = 0;
	lvl = 0;
	while (shell->my_envp[i] != NULL)
	{
		if (ft_strncmp(shell->my_envp[i], "SHLVL=", 6) == 0)
		{
			lvl = ft_atoi(shell->my_envp[i] + 6);
			lvl++;
			break ;
		}
		i++;
	}
	shell->my_envp[i] = do_alloc(&gc->shell, 7 + num_len(lvl), \
		TYPE_SINGLE_PTR, "shllvl");
	if (!shell->my_envp[i])
		return (-1);
	init_shllvl(shell->my_envp[i], lvl);
	printf("%s\n", shell->my_envp[i]);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:34:00 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/18 11:59:34 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_built_in(char *value)
{
	
	if ((ft_strcmp(value, "cd") == 0)
		|| (ft_strcmp(value, "echo") == 0)
		|| (ft_strcmp(value, "export") == 0)
		|| (ft_strcmp(value, "unset") == 0)
		|| (ft_strcmp(value, "env") == 0)
		|| (ft_strcmp(value, "pwd") == 0)
		|| (ft_strcmp(value, "exit") == 0))
		return (1);
	else
		return (0);
}

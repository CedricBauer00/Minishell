/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_BUILTINS.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:34:00 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/31 10:49:06 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_built_in(t_main *main)
{
	if ((ft_strncmp(main->word, "cd", 2) == 0 && main->word[2] == '\0')\
		|| (ft_strncmp(main->word, "echo", 4) == 0 && main->word[4] == '\0') \
		|| (ft_strncmp(main->word, "export", 6) == 0 && main->word[6] == '\0') \
		|| (ft_strncmp(main->word, "unset", 5) == 0 && main->word[5] == '\0') \
		|| (ft_strncmp(main->word, "env", 3) == 0 && main->word[3] == '\0') \
		|| (ft_strncmp(main->word, "pwd", 3) == 0 && main->word[3] == '\0') \
		|| (ft_strncmp(main->word, "exit", 4) == 0 && main->word[4] == '\0') )
		return (1);
	else
		return (0);
}

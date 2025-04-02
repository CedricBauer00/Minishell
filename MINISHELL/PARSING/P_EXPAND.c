/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_EXPAND.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:58:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/02 10:23:04 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	expands(t_main *main, int *i)
{
	int	ws;
	
	ws = *i;
	(*i)++;
	
	// if (main->line[*i] == '?' || main->line[*i] == '$') this case needs handling ????
	while (ft_isalnum(main->line[*i]) == 0 && main->line[*i] == '_')
		*i++;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_PIPE.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:12:45 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/01 10:19:37 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	pipes(t_main *main, t_gc_list *gc_list, int i)
{
	main->error = create_token(&main->tokens, TOKEN_PIPE, "|", gc_list);
	return (++i);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_REDIRECTS.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:25:15 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/01 10:37:16 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	redirect_in(t_main *main, int i, t_gc_list *gc_list)
{
	main->error = create_token(&main->tokens, TOKEN_REDIRECT_IN, "<", gc_list);
	return (++i);
}

int	redirect_out(t_main *main, int i, t_gc_list *gc_list)
{
	main->error = create_token(&main->tokens, TOKEN_REDIRECT_OUT, ">", gc_list);
	return (++i);
}

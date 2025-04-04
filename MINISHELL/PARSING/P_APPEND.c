/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_APPEND.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:38:25 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/04 12:09:58 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	append(t_main *main, int i, t_gc_list *gc_list) //NULLCHECKS für main, main.line, gc_list??
{
	if (main->line[i + 2] == '>')
		return (-1);
	main->error = create_token(&main->tokens, TOKEN_APPEND, ">>", gc_list) ;
	return (i += 2);
}

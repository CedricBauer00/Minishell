/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_syntax.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:01:32 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/14 15:22:59 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	validate_syntax(t_token *token)
{
	if (!token)
		return 1;
	t_token *cur;

	cur = token;
	while (cur)
	{
		if ((cur->type & (TOKEN_PIPE | TOKEN_APPEND | TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT)) &&
			(!cur->next || cur->next->type == TOKEN_PIPE || cur->next->type == TOKEN_NONE))
		{
			//perror(RED"syntax error"DEFAULT);
			//todo all_Free 
			return (-1);
		}

		//ensure that pipe is not the beginning or end
		else if (cur->type == TOKEN_PIPE)
		{
			if(!cur->prev || !cur->next || cur->next->type == TOKEN_PIPE)
			{
				//perror(RED"syntax error"DEFAULT);
				return (-1);
			}
		}
		
		//heck if a TOKEN_WORD follows after a redirection
		else if (cur->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND))
		{
			if (!cur->next || cur->next->type != TOKEN_WORD)
			{
				//perror(RED"syntax error"DEFAULT);
				return (-1);
			}
		}

		//heck if a TOKEN_EOF follows after a TOKEN_HEREDOC
		else if (cur->type == TOKEN_HEREDOC)
		{
			if(cur->next->type != TOKEN_WORD)
			{
				return (-1);
			}
		}
		//memo ich muss nicht es hier auschecken, weil TOKEN_FLAGS mit ACCESS funktion ueberprueft werden kann.
		// if (cur->type == TOKEN_CMD || cur->type == TOKEN_FLAGS)
		// {
		// 	cur->next file.
		// 	cur->next flags 
		// 	cur->next >>
		// 	cur->next <<
		// 	cur->next <
		// 	cur->next >
		// 	cur->next |
		// 	cur->next 
		// }
		cur = cur->next;
	}
	return (1);
}

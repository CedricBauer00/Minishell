/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_utils4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:06:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/02 15:06:54 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_err(char *s1, char *s2, char *s3, int indic)
{
	if (indic == 0)
		write(2, s1, ft_strlen(s1));
	else if (indic == 1)
	{
		write(2, s1, ft_strlen(s1));
		write(2, s2, ft_strlen(s2));
		write(2, s3, ft_strlen(s3));
	}
}

void	choose_error_statement(int indic, char *value)
{
	if (indic == 1)
		print_err("minishell: syntax error near unexpected token `newline'\n", \
			NULL, NULL, 0);
	if (indic == 2)
		print_err("minihsell: syntax error near unexpected token `", \
			value, "'\n", 1);
	if (indic == 3)
		print_err("minishell: bonus error!\n", NULL, NULL, 0);
	if (indic == 4)
	{
		print_err("minishell: unexpected EOF while looking for matching `", \
			value, "'\n", 1);
		print_err("minishell: syntax error: unexpected end of file\n", \
			NULL, NULL, 0);
	}
}

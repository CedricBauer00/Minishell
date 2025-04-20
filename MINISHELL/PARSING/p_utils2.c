/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:19:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/18 16:07:59 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_char(t_main *main, int i, int indic)
{
	if (indic == 0)
	{
		if (ft_isspace(main->line[i]))
		return (1);
	}
	else
	{
		if (ft_isspace(main->line[i]))
			return (1);
		if (main->line[i] == '|' || main->line[i] == '<' || main->line[i] == '>'
			|| main->line[i] == '$' || main->line[i] == '"' || main->line[i] == '\'')
			return (1);
	}
	return (0);
}

int	check_quote(t_main *main)
{
	int		i;
	int		j;
	char	c;
	
	i = 0;
	j = 0;
	while (main->line[i])
	{
		if (j == 0 && (main->line[i] == '"' || main->line[i] == '\''))
		{
			c = main->line[i];
			j = 1;
		}
		else if (j == 1 && main->line[i] == c)
			j = 0;
		i++;
	}
	if (j == 1)
	{
		if (c == '"')
			return (choose_error_statement(4, "\""), -1);
		else
			return (choose_error_statement(4, "\'"), -1);
	}
	return (0);
}

int is_quote(t_main *main, int i)
{
	int		j;
	char	c;
	
	j = 0;
	--i;
	while (main->line[++i] != '\0')
	{
		if (j == 1 && main->line[i] == c)
			j = 0;
		else if (j == 0 && (main->line[i] == '"' || main->line[i] == '\''))
		{
			c = main->line[i];
			j = 1;
		}
		else if (j == 0 && (main->line[i] == '|' || main->line[i] == '$'
			|| main->line[i] == '<' || main->line[i] == '>'
			|| valid_char(main->line[i], 0) == 1))
			break ;
	}
	return (i);
}

void	set_default(t_main *main)
{
	main->tokens = NULL;
	main->line = NULL;
	main->last_status_exit = 0;
	main->envp = NULL;
	main->next_line = NULL;
	main->new = NULL;
	main->word = NULL;
	main->temp_for_line = NULL;
	main->error = 0;
	return ;
}

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

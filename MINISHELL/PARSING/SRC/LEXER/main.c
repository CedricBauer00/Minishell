/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/17 18:04:19 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

//create_token()

int main(int argc, char **argv)
{
	int	i;
	int	j;
	
	if (argc >= 1)
	{
		while (argv[j] != NULL)
		{
			while (argv[j][i])
			{
				if (argv[j][i] == '|')
					create_token()
				if (argv[j][i] == '<')

				if (argv[j][i] == '>')

				if (argv[j][i] == '')
				i++;
			}
			j++;
		}
	}
	write(1, "\n", 1);
}
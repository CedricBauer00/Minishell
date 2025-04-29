/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 19:01:51 by cbauer            #+#    #+#             */
/*   Updated: 2024/11/08 14:56:31 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	counter;

	counter = 0;
	while (s[counter] != '\0')
	{
		write(fd, &s[counter], 1);
		counter++;
	}
}

// int main()
// {
// 	ft_putstr_fd("", 1);
// 	return (0);
// }

// #include "../libft/libft.h"

// int	ft_putstr_fd(char *s, int fd)
// {
// 	int	counter;

// 	counter = 0;
// 	while (s[counter] != '\0')
// 	{
// 		write(fd, &s[counter], 1);
// 		counter++;
// 	}
// 	return (counter);
// }
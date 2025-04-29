/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:50:21 by cbauer            #+#    #+#             */
/*   Updated: 2024/11/08 14:56:06 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// int main()
// {
// 	ft_putchar_fd('c', 0);
// 	return (0);
// }

// #include "libft.h"

// int	ft_putchar_fd(char c, int fd)
// {
// 	write(fd, &c, 1);
// 	return (1);
// }
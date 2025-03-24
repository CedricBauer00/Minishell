/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndrup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:19:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/21 14:58:09 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCLUDE/libft/libft.h"
#include <stdio.h>


// size_t	ft_strlen(const char *str)
// {
// 	int	counter;

// 	counter = 0;
// 	while (str[counter] != '\0')
// 		counter++;
// 	return (counter);
// }

char	*ft_strndup(const char *str, size_t n)
{
	size_t	len;
	size_t	counter;
	char	*ptr;

	len = ft_strlen(str);
	if (n < len)
		len = n;
	ptr = malloc((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	counter = 0;
	while (counter < len && str[counter] != '\0')
	{
		ptr[counter] = str[counter];
		counter++;
	}
	ptr[counter] = '\0';
	return (ptr);
}

// int main()
// {
// 	int i = 5;
// 	char *str = "Helloooo!";
// 	char *s;

// 	s = ft_strndup(str, i);
// 	printf("%s\n", s);
// 	return (0);
// }
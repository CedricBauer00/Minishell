/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:53:21 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/15 10:33:05 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*gc_strdup(const char *str, t_gc_list *gc_list)
{
	size_t	len;
	size_t	counter;
	char	*ptr;

	if (!str)
		return NULL;
	len = ft_strlen(str);
	ptr = do_alloc(gc_list, len * sizeof(char) + 1, TYPE_SINGLE_PTR);
	if (!ptr)
		return (NULL);
	counter = 0;
	while (str[counter] != '\0')
	{
		ptr[counter] = str[counter];
		counter++;
	}
	ptr[counter] = '\0';
	return (ptr);
}

char	*gc_strndup(const char *str, size_t n, t_gc_list *gc_list)
{
	size_t	len;
	size_t	counter;
	char	*ptr;

	len = ft_strlen(str);
	if (n < len)
		len = n;
	ptr = do_alloc(gc_list, (len + 1) * sizeof(char),TYPE_DOUBLE_PTR);
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

char	*gc_strjoin(char const *s1, char const *s2, t_gc_list *gc_list)
{
	char	*newstr;
	size_t	len;
	size_t	counter;
	size_t	i;

	if (s1[0] == '\0' && s2[0] == '\0')
		return (gc_strdup("", gc_list));
	len = ft_strlen(s1) + ft_strlen(s2);
	newstr = (char *)do_alloc(gc_list, (len + 1) * sizeof(char),TYPE_SINGLE_PTR);
	if (!newstr)
		return (0);
	counter = -1;
	while (s1[++counter] != '\0')
		newstr[counter] = s1[counter];
	i = 0;
	while (s2[i] != '\0')
		newstr[counter++] = s2[i++];
	// if (s2)
	 	// free((void *)s2);
	newstr[counter] = '\0';
	return (newstr);
}

int	valid_char(int c, int indic)
{
	if (indic == 0)
		return (ft_isspace(c));
	else
	{
		if (c == '\'' || c == '"' || c == '|' || c == '<' || c == '>' || c == '$')
			return (1);
		if (ft_isspace(c) == 1)
			return (1);
	}
	return (0);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

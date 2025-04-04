/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_UTILS.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:53:21 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/04 12:38:23 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../INCLUDE/parsing.h"
#include "parsing.h"


int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
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

int	valid_char(int c)
{
	if (c >= 33 && c <= 126)
		return (1);
	return (0);
}

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

char	*gc_strjoin(char const *s1, char const *s2, t_gc_list *gc_list)
{
	char	*newstr;
	size_t	len;
	size_t	counter;
	size_t	i;

	if (s1[0] == '\0' && s2[0] == '\0')
	{
		return (gc_strdup("", gc_list));
	}
	// if (strcmp(s2,"") == 0)
	// {
	// 	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	// }
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
	// 	free((void *)s2);
	newstr[counter] = '\0';
	return (newstr);
}

int	check_char(t_main *main, int i, int ind)
{
	if (ind == 0)
	{
		if (ft_isspace(main->line[i]))
			return (0);
	}
	else
	{
		if (ft_isspace(main->line[i]))
			return (0);
		if (main->line[i] == '|' || main->line[i] == '<' || main->line[i] == '>'
			|| main->line[i] == '$' || main->line[i] == '"' || main->line[i] == '\'')
			return (1);
	}
}

int is_quote(t_main *main, int i)
{
	int		j;
	char	c;
	
	j = 0;
	while (1)
	{
		if (j == 0 && (main->line[i] == '"' || main->line[i] == '\''))
		{
			c = main->line[i];
			j = 1;
		}
		else if (j == 0 && )
	}
}
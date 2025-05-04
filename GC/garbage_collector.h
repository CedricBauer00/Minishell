/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:41:48 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/04 16:50:38 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "../libft/libft.h"

# define PURPLE		"\x1b[35m"
# define POWER_PURPLE "\x1b[1;45m"
# define RESET		"\x1b[0m"

typedef enum e_data_type
{
	TYPE_SINGLE_PTR,
	TYPE_DOUBLE_PTR,
	TYPE_STRUCT,
	TYPE_END
}	t_data_type;

typedef struct s_gc_list
{
	void				*data;
	struct s_gc_list	*next;
	t_data_type			type;
	char				*id;
	int					level;
}	t_gc_list;

typedef struct s_gc
{
	t_gc_list	*temp;
	t_gc_list	*shell;
}	t_gc;

// ----------------------------------------------------------------------
//								  gc1.c
// ----------------------------------------------------------------------

t_gc		*get_gc(void);
t_gc		*init_gc(void);
t_gc_list	*init_gc_list(void);
void		*do_alloc(t_gc_list **gc_lst, size_t howmuch, \
		t_data_type data_type, char *id);
void		free_data_by_type(void *data, t_data_type data_type);

// ----------------------------------------------------------------------
//								  gc2.c
// ----------------------------------------------------------------------

t_gc_list	*find_node(t_gc_list *gc_lst, void *target);
t_gc_list	*find_node_with_id(t_gc_list *gc_lst, char *id);
void		delete_node(t_gc_list **gc_lst, t_gc_list *to_delete);
void		all_free(t_gc_list **gc_lst);
void		gc_level_up(t_gc_list *gc_list);

// ----------------------------------------------------------------------
//								  gc3.c
// ----------------------------------------------------------------------

void		gc_free_by_level(t_gc_list *gc_list);
void		gc_free(t_gc *gc);
void		print_list(t_gc_list **gc_lst);
void		print_gc(t_gc *gc);
//char		*gc_strdup(const char *str, t_gc_list **gc_lst);

//	gc_utils.c
int		ft_strcmp(char *s1, char *s2);
char	*gc_strdup(const char *str, t_gc_list **gc_list);
char	*gc_strndup(const char *str, size_t n, t_gc_list **gc_list);
char	*gc_strjoin(char const *s1, char const *s2, t_gc_list **gc_list);

#endif

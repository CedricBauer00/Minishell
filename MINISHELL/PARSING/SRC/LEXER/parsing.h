/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:08:10 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/18 11:04:03 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "../../../INCLUDE/libft/libft.h"
// ----------------------------------------------------------------------
//								LEXER
// ----------------------------------------------------------------------

typedef enum
{
	TOKEN_WORD,			//Command / word
	TOKEN_PIPE,			//Symbol: |
	TOKEN_REDIRECT_IN,	//Symbol: <
	TOKEN_REDIRECT_OUT,	//Symbol: >
	TOKEN_APPEND,		//Symbol: >>
	TOKEN_HEREDOC,		//Symbol: <<
	TOKEN_QUOTE,		//String in '
	TOKEN_DQOUTE,		//string in "
	TOKEN_EOF			//End of input
}	t_token_type;


typedef struct s_token //struct being allocated for each token from input
{
	t_token_type	type;
	char			*value;
	struct			s_token *next;
}	t_token;

typedef struct s_main
{
	t_token	*start; //= first
}	t_main;

// like this 

// t_token *first;

// first = malloc(sizeof(t_token));
// first->type = TOKEN_WORD;
// first->value = strdup("echo");
// first->next = NULL;

#endif
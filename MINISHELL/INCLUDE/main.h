/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:08:10 by cbauer            #+#    #+#             */
/*   Updated: 2025/03/19 16:51:04 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

//# include <stdio.h> //already have in gabae_colletor.h 
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include "../gabage_manager/gabage_collector.h"

//temp header
#include <string.h>

//int last_status_exit; //to check last exited status.
# include "libft/libft.h"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define DEFAULT "\033[0m"


typedef struct s_token
{
	TOKEN_WORD,			//word
	TOKEN_CMD,			//Command
	TOKEN_FLAGS,		// -l, -a
	TOKEN_BUILT_IN,		// cd, pwd, export, ...
	TOKEN_PIPE,			//Symbol: |
	TOKEN_REDIRECT_IN,	//Symbol: <
	TOKEN_REDIRECT_OUT,	//Symbol: >
	TOKEN_APPEND,		//Symbol: >>
	TOKEN_HEREDOC,		//Symbol: <<
	TOKEN_QUOTE,		//String in '
	TOKEN_DQOUTE,		//string in "
	TOKEN_VAR,			//$ variable
	TOKEN_EOF,			//End of input

	SLASH				// bin/ls
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
	char	**envp;
}	t_main;

typedef struct s_cmd
{
	int				last_status_exit;
	char			*cmd;
	char			*flags;  //idont know could be delte
	struct s_cmd	*next;   //"ls -l"
}t_cmd;



// like this 

// t_token *first;

// first = malloc(sizeof(t_token));
// first->type = TOKEN_WORD;
// first->value = strdup("echo");
// first->next = NULL;

// ----------------------------------------------------------------------
//								LEXER
// ----------------------------------------------------------------------

int		main(int argc, char **argv);
int		create_token(t_token **tokens, t_token_type type, char *str);
int		append_token(t_token **tokens, t_token *new_token);
void	free_tokens(t_token *tokens);
void	set_default(t_main *main);

// ---------------------------Lexer_utils--------------------------------

int		ft_isspace(char c);


#endif
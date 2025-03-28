/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/28 12:21:10 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

//# include <stdio.h> //already have in gabae_colletor.h

// # include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
// # include <stdio.h>
# include "/Users/cbauer/Documents/03_Circle/privat_mini/MINISHELL/PARSING/GC/garbage_collector.h"
# include <readline/readline.h>
# include <readline/history.h>

# include "libft/libft.h"


#define RED "\033[0;31m"    //error
#define GREEN "\033[0;32m"  //
#define BLUE "\033[0;34m"   //ok
#define YELLOW "\033[0;33m" //
#define DEFAULT "\033[0m"   //

typedef enum s_tenum
{
    TOKEN_WORD = 0,         //word
    TOKEN_CMD = 1,          //Command
    TOKEN_ARG = 2,
    TOKEN_FLAGS = 3,        // -l, -a
    TOKEN_BUILT_IN = 4,     // cd, pwd, export, ...
    TOKEN_PIPE = 5,         //Symbol: |
    TOKEN_REDIRECT_IN = 6,  //Symbol: <
    TOKEN_REDIRECT_OUT = 7, //Symbol: >
    TOKEN_APPEND = 8,       //Symbol: >>
    TOKEN_HEREDOC = 9,      //Symbol: <<
    TOKEN_QUOTE = 10,        //String in '
    TOKEN_DQOUTE = 11,       //string in "
    TOKEN_VAR = 12,          //$ variable
    TOKEN_EOF = 13,          //End of input
    SLASH               // bin/ls
}   t_token_type;

typedef struct s_token //struct being allocated for each token from input
{
    t_token_type    type;
    char            *value;
    struct          s_token *next;
}   t_token;

typedef struct s_main
{
    t_token	*tokens;
    char	**envp;
	int		last_status_exit;
	char    *line;
	char    *next_line;
    char    *new;
	char    *word;
    char    *old_line;
    char    *temp;

}   t_main;

typedef struct s_cmd
{
    char            *cmd;
    char            *flags;  //idont know could be delte
    struct s_cmd    *next;   //"ls -l"
}t_cmd;

// like this
// t_token *first;
// first = malloc(sizeof(t_token));
// first->type = TOKEN_WORD;
// first->value = strdup("echo");
// first->next = NULL;

// ----------------------------------------------------------------------
//                              LEXER
// ----------------------------------------------------------------------

int     main(int argc, char **argv, char **envp);
int     create_token(t_token **tokens, t_token_type type, char *str);
int     append_token(t_token **tokens, t_token *new_token);
void    free_tokens(t_token *tokens);
void    set_default(t_main *main);

// ----------------------------------------------------------------------
// 							  Lexer_utils
// ----------------------------------------------------------------------

int     ft_isspace(char c);
char	*ft_strndup(const char *str, size_t n);
int	valid_char(int c);

char	**copy_envp(t_gc_list *gc_lst, char **envp);
int	get_envp_count(char **envp);




// copy_envp.c  -- ok!
// char    **copy_envp(t_gc_list *gc_lst, char **envp);
// int     get_envp_count(char **envp);
//built_in
//pwd
//cd
#endif
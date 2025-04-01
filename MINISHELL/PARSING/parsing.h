/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/01 10:45:29 by cbauer           ###   ########.fr       */
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
// # include "/Users/cbauer/Documents/03_Circle/privat_mini/MINISHELL/PARSING/GC/garbage_collector.h"
# include "GC/garbage_collector.h"

# include <readline/readline.h>
# include <readline/history.h>

# include "libft.h"


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
    TOKEN_VAR = 10,          //$ variable
    TOKEN_EOF = 11,          //End of input
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
	char	*line;
    char    *temp_for_line;
	char	*next_line;
	char	*new;
	char	*word;
	char	*old_line;
	char	*temp;
	int     error;

}   t_main;

typedef struct s_cmd
{
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
//                              LEXER
// ----------------------------------------------------------------------

int     main(int argc, char **argv, char **envp);
// int     create_token(t_token **tokens, t_token_type type, char *str);
int	create_token(t_token **tokens, t_token_type type, char *str, t_gc_list *gc_list);

int     append_token(t_token **tokens, t_token *new_token);
void    free_tokens(t_token *tokens);
void	set_default(t_main *main);

// ----------------------------------------------------------------------
// 							  Lexer_utils
// ----------------------------------------------------------------------

int     ft_isspace(char c);
// char	*ft_strndup(const char *str, size_t n);
char	*gc_strndup(const char *str, size_t n, t_gc_list *gc_list);
char	*gc_strdup(const char *str, t_gc_list *gc_list);
char	*gc_strjoin(char const *s1, char const *s2, t_gc_list *gc_list);
int     valid_char(int c);
char	**copy_envp(t_gc_list *gc_lst, char **envp);
int	    get_envp_count(char **envp);

// ----------------------------------------------------------------------
// 							  BUILT-INS
// ----------------------------------------------------------------------

int	is_built_in(t_main *main);

// ----------------------------------------------------------------------
// 							SINGLE_QUOTES
// ----------------------------------------------------------------------

int	quotes(t_main *main, int ws, int i, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							DOUBLE_QUOTES
// ----------------------------------------------------------------------

int	dquotes(t_main *main, int ws, int i, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							DOUBLE_QUOTES
// ----------------------------------------------------------------------

int	pipes(t_main *main, t_gc_list *gc_list, int i);

// ----------------------------------------------------------------------
// 							REDIRECT_IN
// ----------------------------------------------------------------------

int	redirect_in(t_main *main, int i, t_gc_list *gc_list); //<

// ----------------------------------------------------------------------
// 							REDIRECT_OUT
// ----------------------------------------------------------------------

int	redirect_out(t_main *main, int i, t_gc_list *gc_list); //>

// ----------------------------------------------------------------------
// 							  APPEND
// ----------------------------------------------------------------------

int	append(t_main *main, int i, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							 HEREDOC
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// 							EXPAND
// ----------------------------------------------------------------------
// copy_envp.c  -- ok!
// char    **copy_envp(t_gc_list *gc_lst, char **envp);
// int     get_envp_count(char **envp);
//built_in
//pwd
//cd
#endif
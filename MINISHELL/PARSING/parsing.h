/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/16 15:39:17 by cbauer           ###   ########.fr       */
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
# include <stdio.h>
# include <signal.h>
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

// typedef enum s_tenum
// {
//     TOKEN_WORD = 0,         //word
//     TOKEN_CMD = 1,          //Command
//     TOKEN_ARG = 2,
//     TOKEN_FLAGS = 3,        // -l, -a
//     TOKEN_BUILT_IN = 4,     // cd, pwd, export, ...
//     TOKEN_PIPE = 5,         //Symbol: |
//     TOKEN_REDIRECT_IN = 6,  //Symbol: <
//     TOKEN_REDIRECT_OUT = 7, //Symbol: >
//     TOKEN_APPEND = 8,       //Symbol: >>
//     TOKEN_HEREDOC = 9,      //Symbol: <<
//     TOKEN_VAR = 10,          //$ variable
//     TOKEN_EOF = 11,          //End of input
//     SLASH               // bin/ls
// }   t_token_type;

typedef enum s_tenum
{
    TOKEN_NONE = 0x0000,
    TOKEN_WORD = 0x0001,         //word/file
    TOKEN_CMD = 0x0002,          //Command
    TOKEN_ARG = 0x0004,
    TOKEN_BUILT_IN = 0x0008,     // cd, pwd, export, ...
    TOKEN_PIPE = 0x0010,         //Symbol: |
    TOKEN_REDIRECT_IN = 0x0020,  //Symbol: <
    TOKEN_REDIRECT_OUT = 0x0040, //Symbol: >
    TOKEN_APPEND = 0x0080,       //Symbol: >>
    TOKEN_HEREDOC = 0x0100,      //Symbol: <<
    TOKEN_VAR = 0x0200,          //$ variable
    TOKEN_EOF = 0x0400,          //End of input
    TOKEN_FILE = 0x0800,         //file
    TOKEN_SPACES = 0x1000,       //spaces
}   t_token_type;



typedef struct s_token //struct being allocated for each token from input
{
    t_token_type    type;
    char            *value;
    struct          s_token *next;
    struct          s_token *prev;
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

typedef struct s_shell
{
	char	**my_envp;
	char	*cur_dir;
	char	*old_dir;
	int		last_status_exit;
}	t_shell;

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
int	valid_char(int c, int indic);
char	**copy_envp(t_gc_list *gc_lst, char **envp);
int	    get_envp_count(char **envp);


// ----------------------------------------------------------------------
// 							    WORDS
// ----------------------------------------------------------------------

void	words(t_main *main, int *i, int ws, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							  BUILT-INS
// ----------------------------------------------------------------------

int	is_built_in(t_main *main);

// ----------------------------------------------------------------------
// 							SINGLE_QUOTES
// ----------------------------------------------------------------------

int	quotes(t_main *main,int *i, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							DOUBLE_QUOTES
// ----------------------------------------------------------------------

int dquotes(t_main *main, int *i, t_gc_list *gc_list);
int	read_word(t_main *main, int *i, t_gc_list *gc_list);
int	get_spaces(t_main *main, int *i, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							    PIPES
// ----------------------------------------------------------------------

int	pipes(t_main *main, t_gc_list *gc_list, int i);

// ----------------------------------------------------------------------
// 							  REDIRECTS
// ----------------------------------------------------------------------

int	operator(t_main *main, int i, char c, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							   APPEND
// ----------------------------------------------------------------------

int	append(t_main *main, int i, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							   HEREDOC
// ----------------------------------------------------------------------

int	heredoc(t_main *main, int i, t_gc_list *gc_list);
int	check_char(t_main *main, int i, int indic);
int is_quote(t_main *main, int i);
char	*heredoc_input(char *del, t_gc_list *gc_list);
int	heredoc_exec(char *input, t_main *main);

// ----------------------------------------------------------------------
// 							   EXPANDS
// ----------------------------------------------------------------------

int is_valid_char(char c);
int	expands(t_main *main, int *i, int ws, t_gc_list *gc_list);
void	variables(t_main *main, int i, int ws, int len, t_gc_list *gc_list);
int	special_character(t_main *main, int *i, char *value, t_gc_list *gc_list);
int	expand_helper(t_main *main, int *i, int ws, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							 SHELL_INFO
// ----------------------------------------------------------------------

t_shell	*init_shell_info(t_gc_list *gc_list);
t_shell *get_shell(t_gc_list *gc_list);



// -------printing-------
void print_tokens(t_token *tokens);
void print_token(t_token *tokens);

int	check_for_node_spaces(t_main *main, t_token *temp, t_gc_list *gc_list);
int	node_spaces_helper(t_token *temp, t_gc_list *gc_list);
int	check_quote(t_main *main);
int	validate_syntax(t_token *token);
void null_node_all_free(t_gc_list **gc_lst);
void	signal_func(int sig);
int	node_space_else_if(t_token **temp, t_gc_list *gc_list);
// int	check_operator(t_main *main, int *i, t_gc_list *gc_list);
// int	check_operator2(t_main *main, int *i, t_gc_list *gc_list);

// copy_envp.c  -- ok! 
// char    **copy_envp(t_gc_list *gc_lst, char **envp);
// int     get_envp_count(char **envp);
//built_in
//pwd
//cd
#endif
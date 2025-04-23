/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/23 14:20:20 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <stdio.h>
# include <signal.h>
# include <termios.h>
# include "GC/garbage_collector.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"
# define DEFAULT "\033[0m"

typedef enum s_tenum
{
	TOKEN_NONE = 0x0000,
	TOKEN_ARG = 0x0002,				//word
	TOKEN_FILE = 0x0001,			//file
	TOKEN_BUILT_IN = 0x0004,		// cd, pwd, export, ...
	TOKEN_PIPE = 0x008,				//Symbol: |
	TOKEN_REDIRECT_IN = 0x0010,		//Symbol: <
	TOKEN_REDIRECT_OUT = 0x0020,	//Symbol: >
	TOKEN_APPEND = 0x0040,			//Symbol: >>
	TOKEN_HEREDOC = 0x0080,			//Symbol: <<
	TOKEN_VAR = 0x0100,				//$ variable
	TOKEN_BONUS = 0x0200,			//End of input
	TOKEN_SPACES = 0x0400,			//spaces
}	t_token_type;

typedef struct s_token //struct being allocated for each token from input
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_main
{
	t_token	*tokens;
	char	**envp;
	int		last_status_exit;
	char	*line;
	char	*temp_for_line;
	char	*next_line;
	char	*new;
	char	*word;
	char	*old_line;
	char	*temp;
	int		error;

}	t_main;

typedef struct s_cmd
{
	char			*cmd;
	char			*flags; // idont know could be delte
	struct s_cmd	*next; //"ls -l"
}	t_cmd;

typedef struct s_shell
{
	char	**my_envp;
	char	*cur_dir;
	char	*old_dir;
	int		last_status_exit;
}	t_shell;

// ----------------------------------------------------------------------
//							  MAIN
// ----------------------------------------------------------------------

int		main_helper(t_main *main, t_gc_list *gc_list);
int		check_operator2(t_main *main, int *i, t_gc_list *gc_list);
int		check_operator(t_main *main, int *i, t_gc_list *gc_list);
int		main_loop(t_main *main, int i, t_gc *gc);
int		main(int argc, char **argv, char **envp);
int		main_loop_helper(t_main *main, int indic, t_gc *gc);


// ----------------------------------------------------------------------
// 							   UTILS_1
// ----------------------------------------------------------------------

void	set_default(t_main *main);
void	print_err(char *s1, char *s2, char *s3, int indic);
void	choose_error_statement(int indic, char *value);

// ----------------------------------------------------------------------
// 								UTILS_2
// ----------------------------------------------------------------------

int		check_char(t_main *main, int i, int indic);
int		check_quote(t_main *main);
int		is_quote(t_main *main, int i);
int		valid_char(int c, int indic);
int		ft_isspace(char c);

// ----------------------------------------------------------------------
// 								UTILS_3
// ----------------------------------------------------------------------

int		ft_strcmp(char *s1, char *s2);
char	*gc_strdup(const char *str, t_gc_list *gc_list);
char	*gc_strndup(const char *str, size_t n, t_gc_list *gc_list);
char	*gc_strjoin(char const *s1, char const *s2, t_gc_list *gc_list);
int		is_valid_char(char c);

// ----------------------------------------------------------------------
// 								ENVP
// ----------------------------------------------------------------------

char	**copy_envp(t_gc_list *gc_lst, char **envp);
int		get_envp_count(char **envp);

// ----------------------------------------------------------------------
// 							 SHELL_INFO
// ----------------------------------------------------------------------

t_shell	*init_shell_info(t_gc_list *gc_list);
t_shell	*get_shell(t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 								SIGNAL
// ----------------------------------------------------------------------

void	signal_func(int sig);
int		ttyattr(void);

// ----------------------------------------------------------------------
// 								WORDS
// ----------------------------------------------------------------------

void	words(t_main *main, int *i, int ws, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							  BUILT-INS
// ----------------------------------------------------------------------

int		is_built_in(char *value);

// ----------------------------------------------------------------------
// 							SINGLE_QUOTES
// ----------------------------------------------------------------------

int		quotes(t_main *main, int *i, t_gc_list *gc_list);
int		quotes_helper(t_main *main, int *i, int ws, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							DOUBLE_QUOTES
// ----------------------------------------------------------------------

int		dquotes(t_main *main, int *i, t_gc_list *gc_list);
int		dquotes_helper(t_main *main, int *i, t_gc_list *gc_list);
int		read_word(t_main *main, int *i, int k, t_gc_list *gc_list);
int		get_spaces(t_main *main, int *i, int k, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 								PIPES
// ----------------------------------------------------------------------

int		pipes(t_main *main, t_gc_list *gc_list, int i);

// ----------------------------------------------------------------------
// 							  REDIRECTS
// ----------------------------------------------------------------------

int		operator(t_main *main, int i, char c, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							   APPEND
// ----------------------------------------------------------------------

int		append(t_main *main, int i, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							   HEREDOC
// ----------------------------------------------------------------------

int		heredoc(t_main *main, int i, t_gc_list *gc_list);
char	*heredoc_input(char *del, t_gc_list *gc_list);
int		heredoc_exec(char *input, t_main *main);
char	*del_quote(char *word, int i, t_gc_list *gc_list);
int		heredoc_quote_helper(char *word);

// ----------------------------------------------------------------------
// 							   EXPANDS
// ----------------------------------------------------------------------

char	*check_for_var(t_main *main, char *var, int len, t_gc_list *gc_list);
int		only_dollar_sign(t_main *main, t_gc_list *gc_list);
int		expand_helper(t_main *main, int *i, int ws, t_gc_list *gc_list);
int		special_character(t_main *main, int *i, \
			char *value, t_gc_list *gc_list);
int		expands(t_main *main, int *i, int ws, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							  SET_UP_TOKENS
// ----------------------------------------------------------------------

int		append_token(t_token **tokens, t_token *new_token);
int		create_token(t_token **tokens, t_token_type \
		type, char *str, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 							  TOKEN_HANDLING
// ----------------------------------------------------------------------

int		node_spaces_helper(t_token *temp, t_gc_list *gc_list);
int		node_space_else_if(t_token **temp, t_gc_list *gc_list);
int		check_tokens(t_token *temp);
int		check_for_node_spaces(t_main *main, t_token *temp, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 								SORTING
// ----------------------------------------------------------------------

int		check_for_command(t_token *tokens);
void	lex_tokens_correctly(t_token *tokens);

// ----------------------------------------------------------------------
// 								SYNTAX
// ----------------------------------------------------------------------

int		syntax_redirects(t_token **cur);
int		syntax_helper(t_token **cur);
int		validate_syntax(t_token *token, t_gc_list *gc_list);

// ----------------------------------------------------------------------
// 								PRINTING
// ----------------------------------------------------------------------

void	print_tokens(t_token *tokens);
void	print_token(t_token *tokens);

#endif
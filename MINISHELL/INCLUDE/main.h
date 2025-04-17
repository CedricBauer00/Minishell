#ifndef MAIN_H
# define MAIN_H

//# include <stdio.h> //already have in gabae_colletor.h 
//# include <stdlib.h> //already have in garbage_colletor.h
// # include <stdbool.h> also in garbage_header
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include "../garbage_manager/garbage_collector.h"

# include "libft/libft.h"
#define RED "\033[0;31m"	//error
#define GREEN "\033[0;32m"	//
#define BLUE "\033[0;34m"	//ok
#define YELLOW "\033[0;33m"	//
#define DEFAULT "\033[0m"	//

typedef enum s_tenum
{
	TOKEN_NONE	= 0x0000,  			//0000 0000
	TOKEN_ARGS	= 0x0001,
	TOKEN_FILE  = 0x0002,

	TOKEN_BUILT_IN = 0x0004,		// cd, pwd, export, ...
	TOKEN_PIPE = 0x008,			//Symbol: |
	TOKEN_REDIRECT_IN = 0x0010,		//Symbol: <
	TOKEN_REDIRECT_OUT = 0x0020,	//Symbol: >
	TOKEN_APPEND = 0x0040,			//Symbol: >>
	TOKEN_HEREDOC = 0x0080,			//Symbol: <<

	TOKEN_VAR = 0x0100,				//$variable
	//TOKEN_EOF = 0x0400,			//End of input
}	t_token_type;

// typedef enum	e_pipe_stage
// {
// 	NONE,
// 	FIRST,
// 	MIDDLE,
// 	LAST,
// }	t_pipe_stage;

// typedef struct s_token_type
// {
// 	IS_WORD;
// 	TOKEN_CMD;		//Command
// 	TOKEN_FLAGS;	// -l, -a
// 	TOKEN_BUILT_IN;		// cd, pwd, export, ...
// 	TOKEN_PIPE;			//Symbol: |
// 	TOKEN_REDIRECT_IN;	//Symbol: <
// 	TOKEN_REDIRECT_OUT;	//Symbol: >
// 	TOKEN_APPEND;		//Symbol: >>
// 	TOKEN_HEREDOC;		//Symbol: <<
// 	TOKEN_QUOTE; 	//String in '
// 	TOKEN_DQOUTE;		//string in "
// 	TOKEN_VAR;		//$ variable
// 	TOKEN_EOF;			//End of input
// }	t_token_type;

// typedef struct s_main
// {
// 	t_token	*start;
// 	t_shell *shell;
// }	t_main;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd_block //struct being allocated for each token from input
{
	//t_token_type				type;		//todo vilt ,,i can delete?
	char						*built_in; //need to change name
	char						**cmd_flags;
	char						*args;
	struct s_io_streams_list	*io_streams;
	struct s_pipe				*pipe;
	pid_t						*child_pids;
	struct s_cmd_block			*prev;
	struct s_cmd_block			*next;
}	t_cmd_block;
/***********************************************************************/

typedef struct s_io_streams_list //> < > <
{
	char						*infile_name;
	char						*outfile_name;
	int							heredoc_fd;
	char						*heredoc_eof;
	char						*heredoc_file;
	int							fd_org_read;
	int							fd_org_write;
	int							fd_in_file;
	int							fd_out_file;
	struct s_io_streams_list	*next;
}	t_io_streams_list;


typedef struct s_pipe
{
	int					*pipefd;
	int					prev_read_end_fd; //pipe 초기값은 -1로.
	int					cur_fd_write_end;
	// struct s_pipe		*next; //maybe i dont need list
	// struct s_pipe		*prev; //maybe i dont need list
}	t_pipe;

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
// 								LEXER
// ----------------------------------------------------------------------

// int		main(int argc, char **argv);
// int		create_token(t_token **tokens, t_token_type type, char *str);
// int		append_token(t_token **tokens, t_token *new_token);
// void		free_tokens(t_token *tokens);
// void		set_default(t_main *main);

// ---------------------------Lexer_utils--------------------------------

// int		ft_isspace(char c);

//-----------------------------EXECUTION------------------------------------

//memo init.c
t_shell	*init_shell_struct(void);
t_shell *get_shell(void);
t_cmd_block *init_command_struct(t_gc_list *gc_lst);
t_pipe *init_pipe(t_gc_list *gc_lst);
t_io_streams_list *init_io_stream_struct(t_gc_list *gc_lst);

//memo copy_envp.c  -- ok!
char	**copy_envp(t_gc_list *gc_lst, char **envp);
int		get_envp_count(char **envp);

//memo pwd.c
char	*pwd(void);

//memo cd.c
int		is_valid_dir(const char *path);
int		get_env_count(char **my_envp);
char	**expand_envp(t_shell *shell, char *new_path);
char	*create_new_path(const char *name, const char *value);
int		check_existing(char **my_envp, const char *name);
int		ft_setenv(const char *name, const char *value, int overwrite, t_shell *shell);
void 	cd(char **argv, t_shell *shell, t_gc_list *gc_list);

//memo export.c
int		export(char **argv, t_shell *shell);
void	print_envp(t_shell *shell, char *flag);
char	*extract_name(char *arg);
char	*extract_value(char *arg);
//char	*ft_strchr(char *str, char c);

//memo unset.c
void	unset(char **argv, t_shell *shell);

//memo echo.c
void	ft_echo(char **argv, t_shell *shell, t_gc_list *gc_list);

//memo pipe
void	add_pipe(t_cmd_block **cmd_block, t_gc_list *gc_lst);
bool	is_first_pipe(t_cmd_block *cmd);
bool	is_middle_pipe(t_cmd_block *cmd);
bool	is_last_pipe(t_cmd_block *cmd);
int		first_pipe_cmd(t_cmd_block *cmd_block, t_shell *shell, t_gc_list *gc_lst);
int		middle_pipe_cmd(t_cmd_block *cmd_block, t_shell *shell, t_gc_list *gc_lst);
int		last_pipe_cmd(t_cmd_block *cmd_block, t_shell *shell);
void	processing_pipe(t_cmd_block *cmd, t_shell *shell, t_gc_list* gc_lst);
void	close_pipefd(t_cmd_block *cmd);
void	close_last_pipefd(t_cmd_block *cmd);
void	close_middle_pipefd(t_cmd_block *cmd);
void	close_first_pipefd(t_cmd_block *cmd);

//memo redirection.c
int		handle_re_dir(t_cmd_block *cmd_block);
int		re_dir_out(t_io_streams_list *io_streams);
int		re_dir_in(t_io_streams_list *io_streams);
void	set_io_streams(t_cmd_block *cmd);

//memo exe_utils.c
void	is_exited(void *failed, t_gc_list *gc_lst);

//memo command_list_utils.c
//t_cmd_block *add_token(t_gc_list *gc_lst, char *cmd, char **args, t_token_type type);

//memo grouplize.c
void	grouplize(t_token *token, t_cmd_block **cmd_block, t_gc_list *gc_lst);
t_cmd_block	*merge_to_one_cmd(t_token **token, t_gc_list *gc_lst);

//memo heredoc.c
int	heredoc();

//memo builtin_utils.c
char	*my_getenv(char **my_envp, char *find, size_t find_len, t_gc_list *gc_lst);

//memo validate.c
int		validate_syntax(t_token *token);
bool	is_validate_cmd_block(t_cmd_block *cmd_b);

//memo heredoc.c
int	process_heredoc(t_io_streams_list *io_streams);

//memo execute.c
t_token *create_token(t_token_type type, char *value);
void	execute(t_cmd_block *cmd_block, t_gc_list *gc_lst, t_shell *shell);
void 	is_executable_cmd(t_cmd_block *cmd_block, t_gc_list *gc_lst);
//built_in
//pwd
//cd


#endif

// char *input = readline("prompt> ");
// if (input)
// {
//     char *gc_input = do_alloc(gc_list, strlen(input) + 1);
//     strcpy(gc_input, input);
//     free(input);  // readline의 원본 메모리 해제
//     // gc_input 사용
// }
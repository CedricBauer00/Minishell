#ifndef MAIN_H
# define MAIN_H

//# include <stdio.h> //already have in gabae_colletor.h 
//# include <stdlib.h> //already have in garbage_colletor.h
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

// typedef struct s_main
// {
// 	t_token	*start;
// 	t_shell *shell;
// }	t_main;

//memo may be cedric doesnt need it .
typedef struct s_cmd
{
	int				last_status_exit;
	char			*cmd;
	char			*flags;  //idont know could be delete
	struct s_cmd	*next;   //"ls -l"
}t_cmd;

/***********************************************************************/

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

//init.c
t_shell	*init_shell_info(void);
t_shell *get_shell(void);

// copy_envp.c  -- ok!
char	**copy_envp(t_gc_list *gc_lst, char **envp);
int		get_envp_count(char **envp);

//pwd.c
char	*pwd(void);

//cd.c
int		is_valid_dir(const char *path);
int		get_env_count(char **my_envp);
char	**expand_envp(t_shell *shell, char *new_path);
char	*create_new_path(const char *name, const char *value);
int		check_existing(char **my_envp, const char *name);
int		ft_setenv(const char *name, const char *value, int overwrite, t_shell *shell);
void	cd(char **argv, t_shell *shell);

//export.c
int		export(char **argv, t_shell *shell);
void	print_envp(t_shell *shell, char *flag);
char	*extract_name(char *arg);
char	*extract_value(char *arg);

//unset.c


//built_in
//pwd
//cd
#endif
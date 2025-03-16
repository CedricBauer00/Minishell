# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>

//temp header
#include <string.h>

//int last_status_exit; //to check last exited status.

typedef enum TokenType
{
	PIPE,				// |
	CMD,				// ls, wc
	FLAGS,				// -l, -a
	BUILD_IN,			// cd, pwd, export, ...
	LEFT_REDIRECTION,   //	<
	RIGHT_REDIRECTION,  //	>
	HEREDOC,			//	<<
	APPEND_OUTPUT,		//	>>
	SEMICOLON,			//by line excute commands.
	QUOTE,				// '
	DOUBLE_QUOTE,		// "

	//can manage just with WORD? 
	TILDE, // ~ 
	SLASH, // bin/ls
	OTHER,
	END,
}TokenType;

//"ls -l | grep .txt" 이경우 cmd 는 'ls -l' 과 'grep .txt'

// and token is 'ls, -l, |, grep, .txt ?? 
typedef struct s_token
{
	TokenType		token_type;
	char			*token;
	struct s_token	*next;
}t_token;

typedef struct s_cmd
{
	int				last_status_exit;
	char			*cmd;
	char			*flags;  //idont know could be delte
	struct s_cmd	*next;   //"ls -l"
}t_cmd;

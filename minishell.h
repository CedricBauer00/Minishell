/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:13:36 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/02 16:13:50 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <signal.h>
# include <termios.h>
# include "GC/garbage_collector.h"
# include <readline/readline.h>
# include <readline/history.h>

// # include "libft/libft.h"

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"
# define DEFAULT "\033[0m"

typedef enum s_tenum
{
	TOKEN_NONE = 0x0000,
	TOKEN_FILE = 0x0001,			//file
	TOKEN_ARG = 0x0002,				//word
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

// typedef struct s_shell
// {
// 	char	**my_envp;
// 	int		*pids;
// 	char	*cur_dir;
// 	char	*old_dir;
// 	int		last_status_exit;
// }	t_shell;

// typedef struct s_token
// {
// 	t_token_type	type;
// 	char			*value;
// 	struct s_token	*prev;
// 	struct s_token	*next;
// }	t_token;



#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:58 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/08 16:33:45 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef EXECUTION_H
# define EXECUTION_H

# include "../minishell.h"

typedef struct s_cmd_block //struct being allocated for each token from input
{
	char						*built_in; //memo just check if it is built in or not!
	bool						is_built_in;
	bool						is_external_cmd;
	//int		heredoc_fd;
	char						**args;
	struct s_io_streams_list	*io_streams;
	struct s_pipe				*pipe;
	int							prev_read_end_fd; //pipe 초기값은 -1로.
	int							cur_fd_write_end;
	int							pipe_count;
	struct s_cmd_block			*prev;
	struct s_cmd_block			*next;
}	t_cmd_block;
/***********************************************************************/

typedef struct s_io_streams_list //> < > <
{
	char	*infile_name;
	char	*outfile_name;
	char	*append_file_name;
	char	*heredoc_eof;
	char	*heredoc_file;

	int		fd_org_read;
	int		fd_org_write;
	int		fd_in_file;
	int		fd_out_file;
	struct s_io_streams_list	*next;
}	t_io_streams_list;

typedef struct s_pipe
{
	int					*pipefd;
	// struct s_pipe		*next; //maybe i dont need list
	// struct s_pipe		*prev; //maybe i dont need list
}	t_pipe;

typedef struct s_shell
{
	char	**my_envp;
	int		*pids;
	char	*cur_dir;
	char	*old_dir;
	int		heredoc_fd;
	int		last_status_exit;
}	t_shell;

typedef struct s_token //struct being allocated for each token from input
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;


//memo init.c
t_shell	*init_shell_struct(t_gc_list **gc_lst);
t_shell *get_shell(void);
t_cmd_block *init_command_struct(t_gc *gc);
t_pipe *init_pipe(t_gc *gc);

t_io_streams_list	*init_io_stream_struct(t_gc *gc);

t_cmd_block *get_cmd_block(void);

//memo builtin_utils.c
char	*find_var_in_env(char **my_envp, char *find, size_t find_len, t_gc_list *gc_lst);
bool	is_valid_identifier(const char *name);
char	*create_new_path(const char *name, const char *value);
int		get_env_count(char **my_envp);
int		check_existing(char **my_envp, const char *name);

//memo copy_envp.c  -- ok!
char	**copy_envp(t_gc *gc, char **envp);
int		get_envp_count(char **envp);

//memo env.c
void    ft_env(char **args, t_shell *shell);

//memo pwd.c
char	*my_getcwd();
void	ft_pwd(char **args, t_gc *gc);

//memo ft_setenv.c
void	ft_setenv(const char *name, const char *value, int overwrite, t_shell *shell);

//memo cd.c
int		is_valid_dir(const char *path);

char	**expand_envp(t_shell *shell, char *new_path);

void 	cd(char **args, t_shell *shell, t_gc *gc);

//memo exit.c
void	ft_exit(char **args, t_shell *shell);

//memo export.c
void	export(char **argv, t_shell *shell);
void	print_envp(t_shell *shell);
char	*extract_name(char *arg);
char	*extract_value(char *arg);
//char	*ft_strchr(char *str, char c);

//memo unset.c
void	ft_unset(char **argv, t_shell *shell);

//memo echo.c
void	ft_echo(char **args, t_shell *shell);

//memo pipe
void	add_pipe(t_cmd_block **cmd_block);
bool	is_first_pipe(t_cmd_block *cmd);
bool	is_middle_pipe(t_cmd_block *cmd);
bool	is_last_pipe(t_cmd_block *cmd);
int		first_pipe_cmd(t_cmd_block *cmd_block);
int		middle_pipe_cmd(t_cmd_block *cmd_block);
int		last_pipe_cmd(t_cmd_block *cmd_block);
void	processing_pipe(t_cmd_block *cmd);
void	close_pipefd(t_cmd_block *cmd);
void	close_last_pipefd(t_cmd_block *cmd);
void	close_middle_pipefd(t_cmd_block *cmd);
void	close_first_pipefd(t_cmd_block *cmd);

//memo redirection.c
int		handle_re_dir(t_cmd_block *cmd_block);
int		re_dir_out(t_io_streams_list *io_streams);
int		re_dir_in(t_io_streams_list *io_streams);
int		in_redir_file_open(t_io_streams_list *io_streams, char *in_filename);
int		out_redir_file_open(t_io_streams_list *io_streams, char *out_filename);
void	set_io_streams(t_cmd_block *cmd);
int		append_redir_file_open(t_io_streams_list *io_streams, char *appned_file_name);

//memo exe_utils.c
void	is_exited(void *failed, t_gc *gc);

//memo command_list_utils.c
//t_cmd_block *add_token(t_gc_list *gc_lst, char *cmd, char **args, t_token_type type);

//memo grouplize.c
void	grouplize(t_token *token, t_cmd_block **cmd_block, t_gc *gc);
t_cmd_block	*merge_to_one_cmd(t_token **token, t_gc *gc);

//memo heredoc.c
int		heredoc();
//void	wait_for_heredoc_pid(pid_t heredoc_pid, int status);
int	wait_for_heredoc_pid(pid_t heredoc_pid, int status);

//memo validate.c
//int		validate_syntax(t_token *token);
bool	is_validate_cmd_block(t_cmd_block *cmd_b);
void	validate_check(t_cmd_block *cmd_block);

//memo heredoc.c
void		process_heredoc(t_shell *shell, t_token *token);
//void	execute_heredoc(t_shell *shell, t_token *cur);
int		execute_heredoc(t_shell *shell, t_token *cur);

// ----------------------------------------------------------------------
// 							memo execute.c
// ----------------------------------------------------------------------

void	execute_pipeline(t_cmd_block *cmd_block);
void	execute_single_command(t_cmd_block *cmd_block);
void	do_alloc_pids(t_cmd_block* cmd_block);
int		count_command(t_cmd_block *cmd_block);
void	main_execute(t_cmd_block *cmd_block);

// ----------------------------------------------------------------------
// 							memo execute2.c
// ----------------------------------------------------------------------

void	single_cmd_execute(t_cmd_block *cur, t_gc *gc);
int		heredoc_fd_offset_and_redir(t_cmd_block *cur);
void	execute_builtin(t_cmd_block *cur, t_shell *shell);
void	run_execve(t_cmd_block *cmd_block, t_gc *gc);
void	wait_for_child_and_update_status(int i);

// ----------------------------------------------------------------------
// 							memo execute3.c
// ----------------------------------------------------------------------

void	prevent_zombie_process();
void	execute_child(t_cmd_block *cur, t_gc *gc, t_shell *shell);
void	fork_and_execute(t_cmd_block *cmd_block, t_gc *gc, int *i);
void	hanlde_heredoc(t_cmd_block *cmd_block);


//memo signal.c

void	signal_handler_for_child(int sign);
//built_in
//pwd
//cd

#endif

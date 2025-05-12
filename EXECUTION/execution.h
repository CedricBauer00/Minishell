/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:58 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/12 17:00:51 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H
# define LONGLONGMAX 9223372036854775807LL
# define LONGLONGMIN (-9223372036854775807LL - 1)

# include "../minishell.h"

typedef struct s_cmd_block
{
	char						*built_in;
	bool						is_built_in;
	bool						is_external_cmd;
	char						**args;
	struct s_io_streams_list	*io_streams;
	struct s_pipe				*pipe;
	int							prev_read_end_fd;
	int							cur_fd_write_end;
	int							pipe_count;
	struct s_cmd_block			*prev;
	struct s_cmd_block			*next;
}	t_cmd_block;

typedef struct s_io_streams_list
{
	char						*infile_name;
	char						*outfile_name;
	char						*append_file_name;
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
	int	*pipefd;
}	t_pipe;

typedef struct s_shell
{
	int		stdin_backup;
	int		stdout_backup;
	char	**my_envp;
	int		*pids;
	char	*cur_dir;
	char	*old_dir;
	int		heredoc_fd;
	int		last_status_exit;
	bool	heredoc_expandable;
}	t_shell;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

// ----------------------------------------------------------------------
// 								env.c
// ----------------------------------------------------------------------

void				ft_env(char **args, t_shell *shell);

// ----------------------------------------------------------------------
// 								pwd.c
// ----------------------------------------------------------------------

char				*my_getcwd(void);
void				ft_pwd(char **args, t_gc *gc);

// ----------------------------------------------------------------------
// 							 ft_setenv.c
// ----------------------------------------------------------------------

void				ft_setenv(const char *name, const char *value, \
	int overwrite, t_shell *shell);

// ----------------------------------------------------------------------
// 								cd.c
// ----------------------------------------------------------------------

int					is_valid_dir(const char *path);
void				cd(char **args, t_shell *shell, t_gc *gc);

// ----------------------------------------------------------------------
// 								exit.c
// ----------------------------------------------------------------------

void				ft_exit(char **args, t_shell *shell, t_gc *gc);

// ----------------------------------------------------------------------
// 								export.c
// ----------------------------------------------------------------------

void				export(char **argv, t_shell *shell);
void				print_envp(t_shell *shell);
char				*extract_name(char *arg);
char				*extract_value(char *arg);

// ----------------------------------------------------------------------
// 								unset.c
// ----------------------------------------------------------------------

void				ft_unset(char **argv, t_shell *shell);
void				foo(char *arg, t_shell *shell);

// ----------------------------------------------------------------------
// 								echo.c
// ----------------------------------------------------------------------

void				ft_echo(char **args, bool newline, int i, int j);

// ----------------------------------------------------------------------
// 							 builtin_utils.c
// ----------------------------------------------------------------------

char				*find_var_in_env(char **my_envp, char *find, size_t find_len);
bool				is_valid_identifier(const char *name);
char				*create_new_path(const char *name, const char *value);
int					get_env_count(char **my_envp);
int					check_existing(char **my_envp, const char *name);

// ----------------------------------------------------------------------
// 							 builtin_utils1.c
// ----------------------------------------------------------------------

char				**expand_envp(t_shell *shell, char *new_path);
char				**copy_envp(t_gc *gc, char **envp);
int					get_envp_count(char **envp);
int					is_valid_dir(const char *path);

// ----------------------------------------------------------------------
// 							 init.c
// ----------------------------------------------------------------------

t_shell				*ixnit_shell_struct(t_gc_list **gc_lst);
t_shell				*get_shell(void);
t_cmd_block			*init_command_struct(t_gc *gc);
t_pipe				*init_pipe(t_gc *gc);
t_io_streams_list	*init_io_stream_struct(t_gc *gc);

// ----------------------------------------------------------------------
// 							 heredoc_expand.c
// ----------------------------------------------------------------------
char				*expand_case_in_heredoc(char *line, t_shell *shell);
char				*non_expanded_case(char *line, char *result, int *i, t_gc *gc);
char				*extract_expanded_env(char *line, int *i, t_shell *shell, t_gc *gc);

// ----------------------------------------------------------------------
// 							 heredoc.c
// ----------------------------------------------------------------------
void				process_heredoc(t_shell *shell, t_token *token);
int					execute_heredoc(t_shell *shell, t_token *cur);
void				heredoc_sigint_handler(int sig);
// ----------------------------------------------------------------------
// 							pipe_utils.c
// ----------------------------------------------------------------------

void				add_pipe(t_cmd_block **cmd_block);
bool				is_first_pipe(t_cmd_block *cmd);
bool				is_middle_pipe(t_cmd_block *cmd);
bool				is_last_pipe(t_cmd_block *cmd);

// ----------------------------------------------------------------------
// 							  pipe_close.c
// ----------------------------------------------------------------------

void				close_first_pipefd(t_cmd_block *cmd);
void				close_middle_pipefd(t_cmd_block *cmd);
void				close_last_pipefd(t_cmd_block *cmd);
void				close_pipefd(t_cmd_block *cmd);

// ----------------------------------------------------------------------
// 							 pipe_process.c
// ----------------------------------------------------------------------

void				processing_pipe(t_cmd_block *cmd);
int					first_pipe_cmd(t_cmd_block *command);
int					middle_pipe_cmd(t_cmd_block *command);
int					last_pipe_cmd(t_cmd_block *command);

// ----------------------------------------------------------------------
//							redirection.c
// ----------------------------------------------------------------------

void				set_io_streams(t_cmd_block *cmd);
void				handle_redir(t_io_streams_list	*io_streams);

// ----------------------------------------------------------------------
//						alloc_check.c
// ----------------------------------------------------------------------

void				is_exited(void *failed, t_gc *gc);

// ----------------------------------------------------------------------
//						execute_single.c
// ----------------------------------------------------------------------

void				execute_single_command(t_cmd_block *cmd_block);
void				single_cmd_execute(t_cmd_block *cur, t_gc *gc);
void				execute_single_external_cmd(t_cmd_block *cur, t_gc *gc, t_shell *shell);

// ----------------------------------------------------------------------
// 							temp_execute.c
// ----------------------------------------------------------------------
void				execute_builtin(t_cmd_block *cur, t_shell *shell);
void				main_execute(t_cmd_block *cmd_block);
void				run_execve(t_cmd_block *cmd_block, t_gc *gc);
void				exec_relative_path(char *path, t_cmd_block *cmd_block, \
	t_gc *gc, t_shell *shell);
void				execute_pipeline(t_cmd_block *cmd_block);

// ----------------------------------------------------------------------
// 							execute_helper.c
// ----------------------------------------------------------------------

void				do_alloc_pids(t_cmd_block *cmd_block);
int					count_command(t_cmd_block *cmd_block);
int					heredoc_fd_offset_and_redir(t_cmd_block *cur);
char				*check_path_before_exec(t_shell *shell, t_gc *gc);
void				access_and_exec(char *arg, char **args, t_shell *shell);

// ----------------------------------------------------------------------
//							execute_wait_child.c
// ----------------------------------------------------------------------

void				execute_child(t_cmd_block *cur, t_gc *gc, t_shell *shell);
void				fork_and_execute(t_cmd_block *cmd_block, t_gc *gc, int *i);
void				wait_for_child_and_update_status(int i);
void				prevent_zombie_process(void);

// ----------------------------------------------------------------------
// 							grouplize_helper.c
// ----------------------------------------------------------------------

void				add_io_streams_list(t_io_streams_list **head, \
	t_io_streams_list *new_io_streams);
void				ready_redir_files(t_io_streams_list *new_io_streams, \
	t_token **cur, t_gc *gc);
void				ready_builtin(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc);
void				add_io_streams(t_token **cur, t_cmd_block *new_cmd_block);
void				ready_args(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc, int *i);

// ----------------------------------------------------------------------
// 							grouplize.c
// ----------------------------------------------------------------------

void				grouplize(t_token *token, t_cmd_block **cmd_block, t_gc *gc);
t_cmd_block			*merge_to_one_cmd(t_token **token, t_gc *gc);
void				ready_all(t_cmd_block *new_cmd_block, t_token **cur, t_gc *gc, int *i);
int					count_cmd_block(t_token *token);

#endif

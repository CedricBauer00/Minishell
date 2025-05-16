/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:53:49 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/16 16:33:26 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"
# include "../EXECUTION/execution.h"
# include "../get_next_line/get_next_line.h"

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

// ----------------------------------------------------------------------
//							  MAIN
// ----------------------------------------------------------------------

int		main_helper(t_main *main, t_gc_list **gc_list);
int		main_loop(t_main *main, int i, t_gc *gc, t_shell *shell);
int		main(int argc, char **argv, char **envp);
int		main_loop_helper(t_main *main, int indic, t_gc *gc, t_shell *shell);

// ----------------------------------------------------------------------
// 							   UTILS_1
// ----------------------------------------------------------------------

void	tild(t_main *main, int *i, t_gc_list **gc_list);
void	set_default(t_main *main);
int		check_operator2(t_main *main, int *i, t_gc_list **gc_list);
int		check_operator(t_main *main, int *i, t_gc_list **gc_list);

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
char	*gc_strdup(const char *str, t_gc_list **gc_list);
char	*gc_strndup(const char *str, size_t n, t_gc_list **gc_list);
char	*gc_strjoin(char const *s1, char const *s2, t_gc_list **gc_list);
int		is_valid_char(char c);

// ----------------------------------------------------------------------
// 								UTILS_3
// ----------------------------------------------------------------------

void	print_err(char *s1, char *s2, char *s3, int indic);
void	choose_error_statement(int indic, char *value);

// ----------------------------------------------------------------------
// 							ENVP + SHLLVL
// ----------------------------------------------------------------------

int		get_envp_count(char **envp);
int		num_len(int n);
void	init_shllvl(char *str, int i);
int		incrmnt_shllvl(t_shell *shell, t_gc *gc);

// ----------------------------------------------------------------------
// 								SIGNAL
// ----------------------------------------------------------------------

void	signal_func(int sig);
int		ttyattr(void);

// ----------------------------------------------------------------------
// 								WORDS
// ----------------------------------------------------------------------

void	words(t_main *main, int *i, int ws, t_gc_list **gc_list);

// ----------------------------------------------------------------------
// 							  BUILT-INS
// ----------------------------------------------------------------------

int		is_built_in(char *value);

// ----------------------------------------------------------------------
// 							SINGLE_QUOTES
// ----------------------------------------------------------------------

int		quotes(t_main *main, int *i, t_gc_list **gc_list);
int		quotes_helper(t_main *main, int *i, int ws, t_gc_list **gc_list);

// ----------------------------------------------------------------------
// 							DOUBLE_QUOTES
// ----------------------------------------------------------------------

int		dquotes(t_main *main, int *i, t_gc_list **gc_list);
int		dquotes_helper(t_main *main, int *i, t_gc_list **gc_list);
int		read_word(t_main *main, int *i, int k, t_gc_list **gc_list);
int		get_spaces(t_main *main, int *i, int k, t_gc_list **gc_list);

// ----------------------------------------------------------------------
// 								PIPES
// ----------------------------------------------------------------------

int		pipes(t_main *main, t_gc_list **gc_list, int i);

// ----------------------------------------------------------------------
// 							  REDIRECTS
// ----------------------------------------------------------------------

int		operator(t_main *main, int i, char c, t_gc_list **gc_list);

// ----------------------------------------------------------------------
// 							   APPEND
// ----------------------------------------------------------------------

int		append(t_main *main, int i, t_gc_list **gc_list);

// ----------------------------------------------------------------------
// 							   HEREDOC
// ----------------------------------------------------------------------

int		heredoc(t_main *main, int i, t_gc_list **gc_list);
char	*heredoc_input(char *del, t_gc_list **gc_list);
int		heredoc_exec(char *input, t_main *main);
char	*del_quote(char *word, int i, t_gc_list **gc_list);
int		heredoc_quote_helper(char *word);

// ----------------------------------------------------------------------
// 							   EXPANDS
// ----------------------------------------------------------------------

char	*check_for_var(t_main *main, char *var, int len, t_gc_list **gc_list);
int		only_dollar_sign(t_main *main, t_gc_list **gc_list);
int		expand_helper(t_main *main, int *i, int ws, t_gc_list **gc_list);
int		special_character(t_main *main, int *i, \
			char *value, t_gc_list **gc_list);
int		expands(t_main *main, int *i, int ws, t_gc_list **gc_list);

// ----------------------------------------------------------------------
// 							  SET_UP_TOKENS
// ----------------------------------------------------------------------

int		append_token(t_token **tokens, t_token *new_token);
int		create_token(t_token **tokens, t_token_type \
		type, char *str, t_gc_list **gc_list);

// ----------------------------------------------------------------------
// 							  TOKEN_HANDLING
// ----------------------------------------------------------------------

int		node_spaces_helper(t_token *temp, t_gc_list **gc_list);
int		node_space_else_if(t_token **temp, t_gc_list **gc_list);
int		check_tokens(t_token *temp);
int		check_for_node_spaces(t_main *main, t_token *temp, t_gc_list **gc_list);

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
int		syntax_heredoc(t_shell *shell, t_token *cur);
int		validate_syntax(t_token *token, int heredoc_fd, t_shell *shell);

// ----------------------------------------------------------------------
// 								PRINTING
// ----------------------------------------------------------------------

void	print_tokens(t_token *tokens);
void	print_tokens_helper(t_token *tokens);

#endif
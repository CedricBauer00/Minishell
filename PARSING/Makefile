#NAME    = minishell

GREEN = \033[0;32m
DEFAULT = \033[0m

#CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address #-Dreadline change
LDFLAGS = -lreadline -L$(LIBFT_DIR) -L$(GET_NEXT_LINE_DIR) -lft
CFLAGS = -I/usr/include/readline -Ilibft -Wextra -Wall -Werror -g -fsanitize=address #del 

#INC = -I ../INCLUDE/# ./GC/ change

#RL_FLAG = -l readline  change

#RL_INCLUDE = -I/usr/local/opt/readline/include
#RL_LIB = -L/usr/local/opt/readline/lib

RM = rm -f
CC = cc

NAME = minishell

LIBFT_DIR = ../libft

GET_NEXT_LINE_DIR = ../get_next_line

GET_NEXT_LINE = $(GET_NEXT_LINE_DIR)/get_next_line.a

LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = m_main.c \
		m_utils1.c \
		m_utils2.c \
		m_utils4.c \
		m_shell_init.c \
		m_signal.c \
		m_copy_envp.c \
		m_set_up_tokens.c \
		../GC/gc1.c \
		../GC/gc2.c \
		../GC/gc3.c \
		../GC/gc_utils.c\
		p_words.c \
		p_builtins.c \
		p_quotes.c \
		p_dquotes.c \
		p_expand.c \
		p_operators.c \
		p_heredoc.c \
		m_syntax.c \
		m_token_handling.c \
		m_sorting.c \
		printing.c \
		../EXECUTION/builtin_utils.c \
		../EXECUTION/cd.c \
		../EXECUTION/copy_envp.c \
		../EXECUTION/echo.c \
		../EXECUTION/env.c \
		../EXECUTION/exe_utils.c \
		../EXECUTION/execute.c \
		../EXECUTION/execute2.c \
		../EXECUTION/execute3.c \
		../EXECUTION/export.c \
		../EXECUTION/grouplize.c \
		../EXECUTION/heredoc.c \
		../EXECUTION/init.c \
		../EXECUTION/pipe.c \
		../EXECUTION/pwd.c \
		../EXECUTION/redirection.c \
		../EXECUTION/unset.c \
		../EXECUTION/signal.c

OBJECTS := $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(GET_NEXT_LINE) $(OBJECTS)
	@$(CC) $(CFLAGS) $(LIBFT) $(OBJECTS) $(GET_NEXT_LINE) $(INC) -o $(NAME) $(LDFLAGS) #RL_FLAG
	@echo "$(GREEN)Minishell compiled!$(DEFAULT)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --silent

$(GET_NEXT_LINE):
	@$(MAKE) -C $(GET_NEXT_LINE_DIR) --silent
%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	@$(MAKE) -C $(LIBFT_DIR) $(GET_NEXT_LINE) clean --silent
	@$(RM) $(OBJECTS)
	@echo "$(GREEN)Minishell cleaned successfully!$(DEFAULT)"

fclean: clean
	@$(RM) $(LIBFT)
	@$(RM) $(NAME)
	@$(RM) $(GET_NEXT_LINE)
	@echo "$(GREEN)Minishell cleaned successfully!$(DEFAULT)"

re: fclean all

debug: $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o debug

#NAME    = minishell

GREEN = \033[0;32m
DEFAULT = \033[0m

#CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address #-Dreadline change
LDFLAGS = -lreadline -L$(LIBFT_DIR) -lft#-L$(GET_NEXT_LINE_DIR)
CFLAGS = -I/usr/include/readline -Ilibft -Wextra -Wall -Werror -g #-fsanitize=address #del 

#INC = -I ../INCLUDE/# ./GC/ change

#RL_FLAG = -l readline  change

#RL_INCLUDE = -I/usr/local/opt/readline/include
#RL_LIB = -L/usr/local/opt/readline/lib

RM = rm -f
CC = cc

NAME = minishell

LIBFT_DIR = libft

#GET_NEXT_LINE_DIR = ../get_next_line

#GET_NEXT_LINE = $(GET_NEXT_LINE_DIR)/get_next_line.a

LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = PARSING/m_main.c \
		PARSING/m_utils1.c \
		PARSING/m_utils2.c \
		PARSING/m_utils4.c \
		PARSING/m_signal.c \
		PARSING/m_copy_envp.c \
		PARSING/m_set_up_tokens.c \
		GC/gc1.c \
		GC/gc2.c \
		GC/gc3.c \
		GC/gc_utils.c\
		PARSING/p_words.c \
		PARSING/p_builtins.c \
		PARSING/p_quotes.c \
		PARSING/p_dquotes.c \
		PARSING/p_expand.c \
		PARSING/p_operators.c \
		PARSING/p_heredoc.c \
		PARSING/m_syntax.c \
		PARSING/m_token_handling.c \
		PARSING/m_sorting.c \
		PARSING/printing.c \
		EXECUTION/builtin_utils.c \
		EXECUTION/cd.c \
		EXECUTION/copy_envp.c \
		EXECUTION/echo.c \
		EXECUTION/env.c \
		EXECUTION/exe_utils.c \
		EXECUTION/execute.c \
		EXECUTION/execute2.c \
		EXECUTION/execute3.c \
		EXECUTION/export.c \
		EXECUTION/grouplize_helper.c \
		EXECUTION/grouplize.c \
		EXECUTION/heredoc.c \
		EXECUTION/init_helper.c \
		EXECUTION/init.c \
		EXECUTION/pipe_helper.c \
		EXECUTION/pipe_helper2.c \
		EXECUTION/pipe.c \
		EXECUTION/pwd.c \
		EXECUTION/redirection.c \
		EXECUTION/unset.c \
		EXECUTION/signal.c\
		EXECUTION/exit.c\
		EXECUTION/ft_setenv.c\
		EXECUTION/builtin_utils1.c

OBJECTS := $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	@$(CC) $(CFLAGS) $(LIBFT) $(OBJECTS) $(INC) -o $(NAME) $(LDFLAGS) #RL_FLAG
	@echo "$(GREEN)Minishell compiled!$(DEFAULT)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --silent

#$(GET_NEXT_LINE):
#	@$(MAKE) -C $(GET_NEXT_LINE_DIR) --silent
%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	@$(MAKE) -C $(LIBFT_DIR) $(GET_NEXT_LINE) clean --silent
	@$(RM) $(OBJECTS)
	@echo "$(GREEN)Minishell cleaned successfully!$(DEFAULT)"

fclean: clean
	@$(RM) $(LIBFT)
	@$(RM) $(NAME)
	@echo "$(GREEN)Minishell cleaned successfully!$(DEFAULT)"

re: fclean all

debug: $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o debug

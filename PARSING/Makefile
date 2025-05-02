#NAME    = minishell

GREEN = \033[0;32m
DEFAULT = \033[0m

#CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address #-Dreadline change
LDFLAGS = -lreadline -Llibft -lft -L$(LIBFT_DIR)#del
CFLAGS = -I/usr/include/readline -Ilibft -Wextra -Wall -Werror -g -fsanitize=address #del 

#INC = -I ../INCLUDE/# ./GC/ change

#RL_FLAG = -l readline  change

#RL_INCLUDE = -I/usr/local/opt/readline/include
#RL_LIB = -L/usr/local/opt/readline/lib

RM = rm -f
CC = cc

NAME = minishell

LIBFT_DIR = ../libft


LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = m_main.c \
		m_utils1.c \
		m_utils2.c \
		m_utils3.c \
		m_utils4.c \
		m_shell_init.c \
		m_signal.c \
		m_copy_envp.c \
		m_set_up_tokens.c \
		../GC/gc1.c \
		../GC/gc2.c \
		../GC/gc3.c \
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
		printing.c 

OBJECTS := $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	@$(CC) $(CFLAGS) $(LIBFT) $(OBJECTS) $(INC) -o $(NAME) $(LDFLAGS) #RL_FLAG
	@echo "$(GREEN)LEXER compiled!$(DEFAULT)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --silent

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean --silent
	@$(RM) $(OBJECTS)
	@echo "$(GREEN)LEXER cleaned successfully!$(DEFAULT)"

fclean: clean
	@$(RM) $(LIBFT)
	@$(RM) $(NAME)
	@echo "$(GREEN)LEXER cleaned successfully!$(DEFAULT)"

re: fclean all

debug: $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o debug

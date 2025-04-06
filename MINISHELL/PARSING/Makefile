#NAME    = minishell

GREEN = \033[0;32m
DEFAULT = \033[0m

#CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address #-Dreadline change
LDFLAGS = -lreadline -Llibft -lft #del
CFLAGS = -I/usr/include/readline -Ilibft -Wextra -Wall -Werror  -g -fsanitize=address #del 

#INC = -I ../INCLUDE/# ./GC/ change

#RL_FLAG = -l readline  change

#RL_INCLUDE = -I/usr/local/opt/readline/include
#RL_LIB = -L/usr/local/opt/readline/lib

RM = rm -f
CC = cc

NAME = minishell

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = P_MAIN.c \
		P_WORDS.c \
		P_UTILS.c \
		P_BUILTINS.c \
		P_QUOTES.c \
		P_DQUOTES.c \
		P_REDIRECTS.c \
		P_HEREDOC.c \
		P_EXPAND.c \
		GC/gc.c \
		copy_envp.c \
		init.c \
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

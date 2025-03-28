GREEN = \033[0;32m
DEFAULT = \033[0m

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address #-Dreadline

RL_FLAG = -lreadline

#RL_INCLUDE = -I/usr/local/opt/readline/include
#RL_LIB = -L/usr/local/opt/readline/lib

RM = rm -f
CC = cc

NAME = minishell

LIBFT_DIR = ../INCLUDE/libft

LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = Lexer_main.c \
		Lexer_utils.c \
		GC/gc.c \
		copy_envp.c

OBJECTS := $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) -o $(NAME) $(RL_FLAG)
	@echo "$(GREEN)LEXER compiled!$(DEFAULT)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --silent

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

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

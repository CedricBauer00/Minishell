GREEN = \033[0;32m]
DEFAULT = \033[0m]

CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

RM = rm -f
CC = cc

NAME = minishell

SOURCES = Lexer_main.c \
		Lexer_utils.c	\

OBJECTS := $(SOURCES:.c=.o)

all: $(NAME)

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo "GREENProgramm compiled!DEFAULT"

clean:

	@$(RM) $(OBJECTS)
	@echo "GREENProgramm cleaned successfully!DEFAULT"

fclean: clean

	@$(RM) $(NAME)
	@echo "GREENProgramm cleaned successfully!DEFAULT"

re: fclean all

debug: $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o debug
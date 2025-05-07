NAME = get_next_line.a

CC = cc
COMP_FLAGS = -Wall -Wextra -Werror
INC_DIR = .

AR = ar rcs
RANLIB = ranlib

RM = rm -f

SRC =	get_next_line.c\
		get_next_line_utils.c\

OBJ := $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)
	$(RANLIB) $(NAME)

%.o: %.cc
	$(CC) $(COMP_FLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
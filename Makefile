SRC_DIR := src
OBJ_DIR := obj
CC := cc

# Also flags for the include directories. Yey :) 
CFLAGS = -Wall -Wextra -Werror
HEADER = -IINCLUDE



# Other rules
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

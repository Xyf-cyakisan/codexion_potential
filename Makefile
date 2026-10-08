.PHONY: clean all fclean re

NAME= codexion
OBJ_DIR = Objects
C_FILES = codexion.c \
	   general_utils.c \
	   memory_management.c \
	   memory_management2.c \
	   parsing_utils.c \
	   parsing.c \
	   parsing2.c \
	   heap_utils.c \
	   simulation_utils.c \
	   simulation_utils2.c \
	   simulation_utils3.c \
	   simulation_utils4.c \
	   simulation_actions.c \
	   monitoring.c \
	   simulation.c \
	   simulation2.c

H_FILES = general_utils.h \
		parsing.h \
		structures.h \
		memory_management.h \
		codexion.h

OBJS = $(C_FILES:%.c=$(OBJ_DIR)/%.o)

$(NAME): $(OBJS)
	cc -Wall -Wextra -Werror -pthread $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c $(H_FILES) | $(OBJ_DIR)
	cc -Wall -Wextra -Werror -pthread -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

all: $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all
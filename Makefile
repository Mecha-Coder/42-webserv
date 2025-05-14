#==============================================================
# DECLARATION
#==============================================================

NAME = webserve
CC   = c++
FLAG = -Wall -Wextra -Werror -Wshadow -std=c++98

GREEN = \033[1;32m
RESET = \033[0m

#==============================================================
# FILES
#==============================================================

OBJ_DIR := obj

SRCS := $(wildcard ./class/*/*.cpp) main.cpp


OBJS := $(patsubst %,$(OBJ_DIR)/%.o,$(basename $(SRCS)))

#==============================================================
# BUILD COMMAND
#==============================================================

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAG) -o $@ $^
	@echo "$(GREEN)program name ->$(RESET) ./$(NAME)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAG) -c $< -o $@

leak: $(NAME)
	valgrind --leak-check=full ./$(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all leak clean fclean re

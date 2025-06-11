#==============================================================
# DECLARATION
#==============================================================

NAME = webserv
CC   = c++
FLAG = -Wall -Wextra -Werror -Wshadow -std=c++98

GREEN = \033[1;32m
RESET = \033[0m

#==============================================================
# FILES
#==============================================================

OBJ_DIR := obj

SRCS := $(wildcard ./src/class/*/*.cpp) \
		$(wildcard ./src/logic/*.cpp) \
		$(wildcard ./src/utils/*.cpp)

OBJS := $(patsubst %,$(OBJ_DIR)/%.o,$(basename $(SRCS)))

#==============================================================
# BUILD COMMAND
#==============================================================

all: $(NAME) prep-eval

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

prep-eval:
	@echo "$(GREEN)CGI script: Windows-style (CRLF) -> Unix-style (LF)$(RESET)"
	find . -type f \( -name '*.py' -o -name '*.php' -o -name '*.js' \) -exec sed -i 's/\r$$//' {} +
	@echo "$(GREEN)Change file permission$(RESET)"
	chmod 000 ./website/1/archive/no_permission.pdf

.PHONY: all leak clean fclean re fix-crlf

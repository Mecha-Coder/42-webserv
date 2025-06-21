#==============================================================
# DECLARATION
#==============================================================

NAME = webserv
CC   = c++
FLAG = -Wall -Wextra -Werror -std=c++98

GREEN = \033[1;32m
RESET = \033[0m

#==============================================================
# FILES
#==============================================================

OBJ_DIR := obj

SRCS := $(wildcard ./src/class/*/*.cpp) \
		$(wildcard ./src/logic/*.cpp) \
		$(wildcard ./src/utils/*.cpp) \
		$(wildcard ./src/toml/*.cpp) \

OBJS := $(patsubst %,$(OBJ_DIR)/%.o,$(basename $(SRCS)))

#==============================================================
# BUILD COMMAND
#==============================================================

all: $(NAME) pre-eval
	@echo "$(GREEN)Successfully compiled $(RESET)"
	@echo "$(GREEN)Run program =>$(RESET) ./$(NAME) [config file]"

$(NAME): $(OBJS)
	$(CC) $(FLAG) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

pre-eval:
	@echo "$(GREEN)Prepare files for eval$(RESET)"
	@chmod 777 ./test.sh

	@echo "$(GREEN) => Convert all script from Windows(CRLF) to Unix(LF)$(RESET)"
	@find . -type f \( -name '*.py' -o -name '*.php' -o -name '*.js' -name '*.sh' \) -exec sed -i 's/\r$$//' {} +

	@echo "$(GREEN) => Change file permission for:$(RESET)"

	@echo "    *./website/1/archive/no_permission.pdf"
	@chmod 000 ./website/1/archive/no_permission.pdf

	@echo "    *./website/1/archive/cannot_delete"
	@chmod 555 ./website/1/archive/cannot_delete

	@echo "    *./config/bad/no_permission.toml"
	@chmod 000 ./config/bad/no_permission.toml

post-eval:
	@chmod 777 ./website/1/archive/no_permission.pdf
	@chmod 777 ./website/1/archive/cannot_delete
	@chmod 777 ./config/bad/no_permission.toml

test:	re
	@ ./test.sh

.PHONY: all test clean fclean re pre-eval post-eval

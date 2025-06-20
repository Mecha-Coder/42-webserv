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
	
	@echo "$(GREEN) => Convert CGI script from Windows(CRLF) to Unix(LF)$(RESET)"
	@find . -type f \( -name '*.py' -o -name '*.php' -o -name '*.js' \) -exec sed -i 's/\r$$//' {} +

	@echo "$(GREEN) => Change file permission for no_permission.pdf && /cannot_delete$(RESET)"
	@chmod 000 ./website/1/archive/no_permission.pdf
	@chmod 555 ./website/1/archive/cannot_delete

post-eval:
	@chmod 777 ./website/1/archive/no_permission.pdf
	@chmod 777 ./website/1/archive/cannot_delete

.PHONY: all clean fclean re pre-eval post-eval

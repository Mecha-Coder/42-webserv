# Standard
NAME		=	webserv

# Directories
INC			=	inc/
SRC_DIR		=	src
OBJ_DIR		=	obj
# SRCB_DIR	=	srcb
# OBJB_DIR	=	objb

# Compiler & flags
CC			=	c++
WFLAGS		=	-Wall -Wextra -Werror -std=c++98 -pedantic
IFLAGS		=	-I$(INC)
DSYM		=	-g3
FSAN		=	-fsanitize=address $(DSYM)
CFLAGS		=	$(WFLAGS) $(IFLAGS)
RM			=	rm -f

# Sources & objects
SRC			=	$(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJ			=	$(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))
# SRCB		=	$(shell find $(SRCB_DIR) -type f -name "*.cpp")
# OBJB		=	$(patsubst $(SRCB_DIR)%.cpp, $(OBJB_DIR)%.o, $(SRCB))

# Colors
RED		=	\033[1;31m
GREEN	=	\033[1;32m
YELLOW	=	\033[1;33m
BLUE	=	\033[1;34m
RESET	=	\033[0m

# Build rules
all:		$(NAME)
	@echo "$(GREEN)Successfully compiled!$(RESET)"

$(NAME):	$(OBJ)
	@echo "\n$(BLUE)Building$(RESET)\t$(NAME)"
	@make -sC $(MLX_DIR)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)Compiling\t$(RESET)%-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@

# bonus:		clean $(OBJ) $(OBJB)
# 	@echo "$(BLUE)Linking bonus objects...$(RESET)"
# 	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(OBJB)
# 	@echo "$(GREEN)Successfully compiled bonus!$(RESET)"

# $(OBJB_DIR):
# 	@mkdir -p $(OBJB_DIR)

# $(OBJB_DIR)/%.o: $(SRCB_DIR)/%.cpp | $(OBJB_DIR)
# 	@mkdir -p $(dir $@)
# 	@printf "$(YELLOW)Compiling\t$(RESET)%-33.33s\r" $@
# 	@$(CC) $(CFLAGS) -c $< -o $@

# Scruba dub dub
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
	$(RM) -rf $(OBJ_DIR); \
	echo "$(RED)Deleting$(RESET)\t"$(OBJ_DIR); else \
	echo "$(BLUE)No $(NAME) objects to remove.$(RESET)"; \
	fi;
# @if [ -d $(OBJB_DIR) ]; then \
# $(RM) -rf $(OBJB_DIR); \
# echo "$(RED)Deleting$(RESET)\t$(OBJB_DIR)"; \
# fi;

fclean:	clean
	@if [ -f "$(NAME)" ]; then \
	$(RM) -f $(NAME); \
	echo "$(RED)Deleting$(RESET)\t"$(NAME); else \
	echo "$(BLUE)No executable $(NAME) to remove.$(RESET)"; \
	fi;

re:		fclean all

# Debugging
asan:		CFLAGS	+=	$(FSAN)
asan:	
	@echo "$(YELLOW)Running with AddressSanitizer...$(RESET)"

valgrind:	$(NAME)
	@echo "$(YELLOW)Running with Valgrind...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

test:		re
	@echo "$(YELLOW)Running tests...$(RESET)"

.PHONY:	all bonus clean fclean re asan valgrind test

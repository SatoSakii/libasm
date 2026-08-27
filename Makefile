NAME		=	libasm.a
TEST		=	libasm_test

NASM		=	nasm
NFLAGS		=	-f elf64
AR			=	ar -rcs
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -MMD -MP

OBJS_DIR 	=	.build
SRCS_DIR	=	srcs
TESTS_DIR	=	tests
INC_DIR		=	includes

SRCS		:=	ft_strlen.s \
				ft_strcpy.s \
				ft_strcmp.s \
				ft_write.s \
				ft_read.s \
				ft_strdup.s

TESTS		:=	main.c \
				utils.c \
				guard.c \
				test_strlen.c \
				test_strcpy.c \
				test_strcmp.c \
				test_write.c \
				test_read.c \
				test_strdup.c

SRCS		:=	$(addprefix $(SRCS_DIR)/, $(SRCS))
OBJS		=	$(patsubst $(SRCS_DIR)/%.s, $(OBJS_DIR)/%.o, $(SRCS))

TESTS		:=	$(addprefix $(TESTS_DIR)/, $(TESTS))
TESTS_OBJS	=	$(patsubst $(TESTS_DIR)/%.c, $(OBJS_DIR)/$(TESTS_DIR)/%.o, $(TESTS))

all:	$(NAME)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.s
	@mkdir -p $(dir $@)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$<$(RESET)"
	@$(NASM) $(NFLAGS) $< -o $@

$(OBJS_DIR)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$<$(RESET)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -I$(TESTS_DIR) -o $@ -c $<

$(TEST): $(NAME) $(TESTS_OBJS)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -I$(TESTS_DIR) $(TESTS_OBJS) $(NAME) -o $(TEST)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(TEST)$(RESET)"

test: $(TEST)
	@./$(TEST)

clean:
	@echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  cleaned	$(RED)$(BOLD)$(ITALIC)$(OBJS_DIR)$(RESET)"
	@rm -rf $(OBJS_DIR)

fclean:
	@echo "$(RED)Cleaning executables...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@rm -f $(NAME) $(TEST)

re: fclean all

-include $(TESTS_OBJS:.o=.d)

.PHONY: all clean fclean re test

BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
GRAY		=	\033[90m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r
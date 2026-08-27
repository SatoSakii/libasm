NAME		=	libasm.a

NASM		=	nasm
NFLAGS		=	-f elf64
AR			=	ar -rcs

OBJS_DIR 	=	.build
SRCS_DIR	=	srcs

SRCS		:=	ft_strlen.s \
				ft_strcpy.s \
				ft_strcmp.s \
				ft_write.s \
				ft_read.s \
				ft_strdup.s

SRCS		:=	$(addprefix $(SRCS_DIR)/, $(SRCS))
OBJS		=	$(patsubst $(SRCS_DIR)/%.s, $(OBJS_DIR)/%.o, $(SRCS))

all:	$(NAME)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.s
	@mkdir -p $(dir $@)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$<$(RESET)"
	@$(NASM) $(NFLAGS) $< -o $@

clean:
	@echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  cleaned	$(RED)$(BOLD)$(ITALIC)$(OBJS_DIR)$(RESET)"
	@rm -rf $(OBJS_DIR)

fclean:
	@echo "$(RED)Cleaning executables...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

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
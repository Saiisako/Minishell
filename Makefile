NAME = minishell
CC = cc
FLAGS = -Wall -Werror -Wextra #-g3 -fsanitize=address
RM = rm -rf
LIBFT = library/libft/libft.a
LIBFT_FLAGS = -L./library/libft -lft
RDL_FLAG = -lreadline

SRCS	=	main.c\
			parsing/env.c\
			builtin/env.c\
			builtin/pwd.c\

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m

OBJS = $(SRCS:.c=.o)

TARGET = $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT_FLAGS) $(RDL_FLAG)
	@echo "$(CYAN)██╗      █████╗ ███████╗██╗   ██╗$(RESET)███████╗██╗  ██╗███████╗██╗     ██╗"
	@echo "$(CYAN)██║     ██╔══██╗╚══███╔╝╚██╗ ██╔╝$(RESET)██╔════╝██║  ██║██╔════╝██║     ██║"
	@echo "$(CYAN)██║     ███████║  ███╔╝  ╚████╔╝ $(RESET)███████╗███████║█████╗  ██║     ██║"
	@echo "$(CYAN)██║     ██╔══██║ ███╔╝    ╚██╔╝  $(RESET)╚════██║██╔══██║██╔══╝  ██║     ██║"
	@echo "$(CYAN)███████╗██║  ██║███████╗   ██║   $(RESET)███████║██║  ██║███████╗███████╗███████╗"
	@echo "$(CYAN)╚══════╝╚═╝  ╚═╝╚══════╝   ╚═╝   $(RESET)╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"

$(LIBFT):
	make -C library/libft

.c.o:
	$(CC) $(FLAGS) -c -o $@ $<

all: $(NAME)

clean:
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@make -s -C library/libft clean
	@echo "files erased successfully"

fclean: clean
	@$(RM) $(NAME) $(NAME_BONUS)
	@make -s -C library/libft fclean

re: fclean all

.PHONY: all clean fclean re bonus minilibx

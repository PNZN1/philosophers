SRC_PATH = src
OBJ_PATH = obj
INC_PATH = include

NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address

SRC = main.c parsing.c init.c utils.c actions.c monitor.c

SRC_PRE = $(addprefix $(SRC_PATH)/,$(SRC))
OBJ = $(addprefix $(OBJ_PATH)/,$(SRC_PRE:.c=.o))

INC = $(addprefix -I,$(INC_PATH))

BOLD = \033[1m
GREEN = \033[32;1m
RED	= \033[31;1m
YELLOW	= \033[33;1m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(INC) -o $(NAME) && printf "$(YELLOW)$(BOLD)\rBuild $(NAME)\r\e[35C[OK]\n$(RESET)"

$(OBJ_PATH)/%.o: %.c include/philo.h
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -c $< $(INC) && printf "$(GREEN)$(BOLD)\rCompiling: $(notdir $<)\r\e[35C[OK]\n$(RESET)"

run: all
	@./philo 5 200 180 20

clean:
	@rm -rf $(OBJ_PATH)
	@echo "$(RED)Cleaning philo$(RESET)"

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, fclean, re, all

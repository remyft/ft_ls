# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rfontain <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/28 20:51:07 by rfontain          #+#    #+#              #
#    Updated: 2018/09/29 15:08:07 by rfontain         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

CC = gcc -g3

RM = rm -rf

RED = "\x1b[0;31;40m"
RESET = "\x1b[1;37;40m"
GREY = "\x1b[1;30;40m"
GREEN = "\x1b[1;32;40m"

LIB_PATH = libft
LIB = $(LIB_PATH)/libft.a
LIB_LINK = -L $(LIB_PATH) -lft

INC_DIR = include
INCS = -I $(LIB_PATH)/ -I $(INC_DIR)

SRCS =	src/deal.c			\
		src/ft_ls.c			\
		src/tools.c			\
		src/utiles.c		\
		src/random.c		\
		src/sort_list.c		\
		src/file_tools.c	\
		src/list_tools.c	\
		src/sort_tools.c	\
		src/list_utiles.c	\
		src/flag_handler.c	\
		src/list_handler.c	\
		src/indir_handler.c	\
		src/right_handler.c	\

OK =      $(GREEN)[OK]$(RESET)		

NEWLINE = $(shell echo "")

CFLAGS +=  -Wall -Wextra -Werror

OBJS = $(SRCS:.c=.o)

$(NAME): $(NEWLINE) $(OBJS) $(LIB)
	@$(CC) $(INCS) $^ -o $@ $(LIB_LINK)
	@echo ""
	@echo $(GREY)" Compilling" $(RESET) [ $(NAME) ] $(OK)

%.o: %.c
	@echo $(RED)" ᚘ  "$(RESET) | tr -d '\n'
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 

$(LIB):
	@echo ""
	@echo " " | tr -d '\n'
	@make -C $(LIB_PATH)

all: $(NAME)

clean:
	@$(RM) $(OBJS)
	@make -C $(LIB_PATH) clean
	@echo $(GREY)" Cleaning :" $(RESET) [ $(NAME) ] $(OK)

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIB_PATH) fclean
	@echo $(GREY)" Deleting.." $(RESET) [ $(NAME) ] $(OK)

reclean: clean
	@$(RM) $(NAME)
	@make -C $(LIB_PATH) fclean
	@echo $(GREY)" Deleting.." $(RESET) [ $(NAME) ] $(OK)
	@echo ""

re: reclean all

nn:
	norminette $(LIB_PATH)
	norminette src
	norminette $(INC_DIR)

.PHONY: clean fclean all re

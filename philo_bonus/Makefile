# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/03 16:05:35 by mmravec           #+#    #+#              #
#    Updated: 2025/02/05 22:12:32 by mmravec          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
CC = cc
CFLAGS = -Wall -Werror -Wextra -g -pthread
#-fsanitize=thread

# Source files
SRCS = main.c utils.c parse.c init.c safe_functions.c dinner.c write.c \
	ft_printf.c ft_printf_utils.c \
	monitor.c

OBJS = $(SRCS:.c=.o)

# Rule to build the so_long executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Default target to build everything
all: $(NAME)

# Clean up object files
clean:
	rm -f $(OBJS)

# Clean up object files and the executables
fclean: clean
	rm -f $(NAME)

# Rebuild everything from scratch
re: fclean all

.PHONY: all clean fclean re

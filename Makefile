# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lformank <lformank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/03 16:05:35 by mmravec           #+#    #+#              #
#    Updated: 2025/06/04 14:04:30 by lformank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g
# -pthread -fsanitize=thread

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
LIBFT_DIR = libft

# Source files
SRCS = main.c utils.c token.c token_extraction.c lexer.c history.c \
	builtins.c execution.c ft_strtok.c file_manipulation.c shell_loop.c \
	token_string_extraction.c lexer_special_tokens.c signals.c \
	lexer_process_redirections.c parser.c pipes.c \
	parser_utils.c parser_redir.c parser_error.c env.c cd.c redirection.c \
	expansion.c fd.c export.c token_extracttion_env.c builtin_exit.c \
	cd_utils.c builtin_cd.c env_print.c env_utils.c env_operations.c \
	env_path.c execution_utils.c execution_search.c execution_fork.c \
	execution_command.c expansion_utils.c lexer_utils.c lexer_env_tokens.c \
	parser_assignment.c parser_pipe.c parser_command.c pipes_execution.c\
	lexer_core.c expansion_quotes.c lexer_extraction.c

# Add src/ prefix to source files
SRCS := $(addprefix $(SRC_DIR)/,$(SRCS))

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libft library
LIBFT = $(LIBFT_DIR)/libft.a

# Detect architecture and set paths accordingly
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_M),arm64)
	# M1/M2 Mac
	READLINE_DIR = /opt/homebrew/opt/readline
else
	# Intel Mac
	READLINE_DIR = /usr/local/opt/readline
endif

# Readline flags
READLINE_FLAGS =  -L$(READLINE_DIR)/lib -lreadline -lncurses
READLINE_INCLUDE = -I$(READLINE_DIR)/include

# Include flags
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR) $(READLINE_INCLUDE)

# Rule to build the final minishell executable
$(NAME): $(OBJ_DIR) $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft $(READLINE_FLAGS)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to build libft by calling its Makefile
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Rule to compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Default target to build everything (both server and client)
all: $(NAME)

# Clean up object files
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

# Clean up object files and the executables
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything from scratch
re: fclean all

.PHONY: all clean fclean re

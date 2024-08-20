NAME = minishell
CC = cc
CFLAGS = -g -fsanitize=address

SRCS = srcs/minishell.c parsing/tokenization.c srcs/makers.c parsing/tokens_v2.c parsing/parser.c execution/run_cmd.c execution/get_paths.c \
		 parsing/ft_split.c execution/get_envp.c execution/ft_echo2.c srcs/ft_strjoin.c srcs/env_to_list.c srcs/echo.c parsing/quotes.c \
		 parsing/utils.c parsing/expantion.c execution/dispatcher.c  execution/ft_echo.c execution/signals.c \
		srcs/echo2.c

OBJS = $(SRCS:.c=.o)


all : $(NAME)

%.o :	%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $^ -lreadline -o $@

clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean

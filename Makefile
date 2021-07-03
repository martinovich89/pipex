NAME = pipex

LIB = -L./libft -lft

INC = -I./includes

SRCS = pipex.c

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror #-fsanitize=address -g3

CC = @clang

RM = @rm -f

all : $(NAME)

$(NAME) : $(OBJS)
	@make -C libft
	$(CC) $(FLAGS) $(INC) -o $(NAME) $(OBJS) $(LIB)
	@echo "$(NAME) created"

%.o : %.c
	$(CC) $(FLAGS) $(INC) -o $@ -c $<

clean :
	@make $@ -C libft
	$(RM) $(OBJS)
	@echo "$(OBJS) deleted"

fclean : clean
	$(RM) $(NAME)
	@echo "$(NAME) deleted"

re : fclean all

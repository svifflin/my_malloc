NAME    =	libmy_malloc_`uname`.so
GENERIC =	libmy_malloc.so

SRCS    =       my_malloc.c	\
                container.c	\
		browsing.c	\
		free.c		\
		xfunctions.c

OBJS    =       $(SRCS:.c=.o)

CC      =	gcc

CFLAGS	=	-W -Wall -Wextra -fPIC

$(NAME) : $(OBJS)
	$(CC) -shared $(OBJS) -o $(NAME)
	ln -s $(NAME) $(GENERIC)

all     :       $(NAME)

clean   :
	$(RM) *.o
	$(RM) *~
	$(RM) *#

fclean	:	clean
	$(RM) $(NAME) $(GENERIC)

re	: fclean all



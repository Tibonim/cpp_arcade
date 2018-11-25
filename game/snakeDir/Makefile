CC = gcc

RM = rm -f

CPPFLAGS += -W -fPIC -Wall -Wextra \
	    -I ../include_files/ -I ../../Includes \
	    -std=c++11 -I ../../scoreWriter/include_files \
	    -I ../../encryption/include_files

LDFLAGS = -shared

NAME = ../../games/lib_arcade_snake.so

SRCS =	Snake.cpp \
	../AGame.cpp \
	../../scoreWriter/scoreWriter.cpp \
	../../encryption/encryption.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	mkdir -p ../../games
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

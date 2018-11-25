CC = gcc

RM = rm -f

CPPFLAGS += -W -fPIC -Wall -Wextra \
	    -I ../include_files/ -I ../../Includes \
	    -std=c++11 -I ../../scoreWriter/include_files \
	    -I ../../encryption/include_files

LDFLAGS = -shared

NAME = ../../games/lib_arcade_centipede.so

SRCS =	Centipede.cpp \
	centipedeNode.cpp \
	Obstacle.cpp \
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

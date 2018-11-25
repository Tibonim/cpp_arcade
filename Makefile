NAME		= arcade

LIBSGAME	= ./game/

LIBNCURSES	= ./graph/ncurses/

LIBSDL		= ./graph/sdl/

LIBOPENGL	= ./graph/opengl/

DIR		= ./

DIRCONTROLLER	= ./controller/

DIRENCRYPTION	= ./encryption/

DIRMENU		= ./menu/

DIREXCEPTION	= ./Exception/

SCOREDISPLAYER	= ./scoreDisplayer/

INCLUDEGAME	= ./game/include_files/

INCLUDEGRAPH	= ./graph/include_files/

INCEXCEPTION	= ./Exception/include_files/

INCPROTOCOL	= ./Includes/

INCEVENTHANDLER	= ./EventHandler/include_files/

INCLOADER	= ./loader/

INCEXCEPTION	= ./Exception/include_files/

INCCONTROLLER	= ./controller/include_files/

INCMENU		= ./menu/include_files/

INCSCORE	= ./scoreDisplayer/include_files/

INCENCRYPTION	= ./encryption/include_files/

SRCS		= $(DIR)main.cpp \
		  $(DIRCONTROLLER)Controller.cpp \
		  $(DIRCONTROLLER)Dir.cpp \
		  $(DIREXCEPTION)CustomException.cpp \
		  $(DIREXCEPTION)GraphException.cpp \
		  $(DIREXCEPTION)LoaderError.cpp \
		  $(DIREXCEPTION)GameError.cpp \
		  $(DIREXCEPTION)DirectoryError.cpp \
		  $(DIREXCEPTION)keyEventError.cpp \
		  $(DIRMENU)Menu.cpp \
		  $(DIRMENU)Pause.cpp \
		  $(SCOREDISPLAYER)scoreDisplayer.cpp \
		  $(DIRENCRYPTION)encryption.cpp

CXXFLAGS	= -std=c++11 -Wall -Werror -Wextra -I$(INCPROTOCOL) \
		  -I$(INCLUDEGAME) -I$(INCLUDEGRAPH) -I$(INCPROTOCOL) \
		  -I$(INCEVENTHANDLER) -I$(INCLOADER) -I$(INCEXCEPTION) \
		  -I$(INCCONTROLLER) -I$(INCMENU) \
		  -I$(INCSCORE) -I$(INCENCRYPTION)

LDFLAGS		= -ldl

OBJ		= $(SRCS:.cpp=.o)

all:		 Game Ncurses Sdl  $(NAME) Opengl

Sdl:
	make -C $(LIBSDL)

Game:
	make -C $(LIBSGAME)

Ncurses:
	make -C $(LIBNCURSES)

Opengl:
	make -C $(LIBOPENGL)

$(NAME):	$(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJ)
	make clean -C $(LIBSGAME)
	make clean -C $(LIBNCURSES)
	make clean -C $(LIBSDL)
	make clean -C $(LIBOPENGL)

fclean:	clean
	rm -rf $(NAME)
	make fclean -C $(LIBSGAME)
	make fclean -C $(LIBNCURSES)
	make fclean -C $(LIBSDL)
	make fclean -C $(LIBOPENGL)

re:	fclean all

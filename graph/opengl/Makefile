NAME		= $(PLACE)lib_arcade_opengl.so

PLACE		= ../../lib/

DIR		= ./

DIRCTRL		= ../../controller/

DIREXCEPTION	= ../../Exception/

DIRHANDLER	= ../../EventHandler/

CONTROLLER	= ../../controller/include_files/

EXCEPTION	= ../../Exception/include_files/

INCLUDE		= ../include_files/

LOADER		= ../../loader/

DIRGRAPH	= ../

HANDLER		= ../../EventHandler/include_files/

GAME		= ../../game/include_files/

PROTOCOL	= ../../Includes/

SRC		= $(DIR)GraphOpenGL.cpp \
		  $(DIR)EventOpenGL.cpp \
		  $(DIRGRAPH)AGraph.cpp \
		  $(DIREXCEPTION)CustomException.cpp \
		  $(DIREXCEPTION)GameError.cpp \
		  $(DIREXCEPTION)LoaderError.cpp \
		  $(DIREXCEPTION)DirectoryError.cpp \
		  $(DIREXCEPTION)GraphException.cpp \
		  $(DIREXCEPTION)keyEventError.cpp \
		  $(DIRHANDLER)EventHandler.cpp

CPPFLAGS	= -std=c++11 -I$(EXCEPTION) -I$(INCLUDE) \
		  -I$(LOADER) -I$(HANDLER) -ldl -fPIC \
		  -I$(GAME) -I$(PROTOCOL)  \
		  -Wall -Werror -Wextra

LDFLAGS		= -shared -lGL -lGLU -lglut -lSDL -lSDL_image -lSDL_ttf

OBJ		= $(SRC:.cpp=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
	mkdir -p $(PLACE)
	g++ $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
	rm -rf $(OBJ)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

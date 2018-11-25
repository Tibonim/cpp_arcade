#include <iostream>
#include <sstream>
#include "keyEventError.hh"
#include "EventHandler.hh"
#include "GraphException.hh"
#include "GraphOpenGL.hpp"
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

namespace arcade {
  GraphOpenGL::GraphOpenGL(std::string const&) : _argc{ "init" }
  {
    int	i = 1;

    _isRunning = true;
    window = NULL;
    if (W_WIDTH <= 0 || W_HEIGHT <= 0 || W_WIDTH > 1000 || W_HEIGHT > 1000)
      throw GraphError {"Window too big", "GraphOpenGL"};
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
      throw GraphError {"Can't init SDL", "GraphOpenGL"};
    glutInit(&i, _argc.get());
    _cmdToPush[SDLK_UP] = &AGraph::pushKeyUp;
    _cmdToPush[SDLK_DOWN] = &AGraph::pushKeyDown;
    _cmdToPush[SDLK_RIGHT] = &AGraph::pushKeyRight;
    _cmdToPush[SDLK_LEFT] = &AGraph::pushKeyLeft;
    _cmdToPush[SDLK_SPACE] = &AGraph::pushShoot;
    _cmdToPush[SDLK_ESCAPE] = &AGraph::pushQuit;

    _shift_code[SDLK_AMPERSAND] = '1';
    _shift_code[SDLK_WORLD_73] = '2';
    _shift_code[SDLK_QUOTEDBL] = '3';
    _shift_code[SDLK_QUOTE] = '4';
    _shift_code[SDLK_LEFTPAREN] = '5';
    _shift_code[SDLK_MINUS] = '6';
    _shift_code[SDLK_BACKQUOTE] = '7';
    _shift_code[SDLK_UNDERSCORE] = '8';
    _shift_code[SDLK_WORLD_71] = '9';
    _shift_code[SDLK_WORLD_64] = '0';

    SDL_WM_SetCaption("Arcade", NULL);
    drawWindow();
  }

  GraphOpenGL::~GraphOpenGL()
  {
    glutExit();
    SDL_Quit();
  }

  void		GraphOpenGL::drawWindow()
  {
    if ((window = SDL_SetVideoMode(W_WIDTH, W_HEIGHT, W_BPP, SDL_OPENGL)) == NULL)
      throw GraphError {"Can't set VideoMode", "drawWindow"};
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void		GraphOpenGL::initDrawMap() const
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, W_WIDTH, 0.0f, W_HEIGHT, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  void GraphOpenGL::renderBitmapString(float x, float y, bool select, const std::string& string) const
  {
    if (select)
      glColor3f(1.0f, 0.0f, 1.0f);
    else
      glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(x, y);
    glScalef(90.0f, 90.0f, 90.0f);
    for (unsigned int i = 0; i < string.length(); i++) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string.at(i));
    }
    glPopMatrix();
  }

  void			GraphOpenGL::drawEdges(PointDraw const& pts, bool select)const
  {
    unsigned int	h = 0;
    unsigned int	w = 0;
    float		posX;
    float		posY;

    while (h < pts.height)
      {
	w = 0;
	while (w < pts.width)
	  {
	    posX = ((pts.posX + w) * (W_WIDTH / 60));
	    posY = ((pts.posY + 1 + h) * (W_HEIGHT / 30));
	    posY = W_HEIGHT - posY;
	    glViewport(posX, posY, W_HEIGHT, W_WIDTH);
	    if ((h == 0 || h == pts.height -1) || (w == 0 || w == pts.width -1
						   || w == pts.width - 2)){
	      if (select)
		{
		  glColor3f(1.0f, 1.0f, 0.0f);
		  glRectf(0.0f, 0.0f, (W_WIDTH / 60),
			  (W_HEIGHT / 30));
		}
	      else
		{
		  glColor3f(0.0f, 1.0f, 1.0f);
		  glRectf(0.0f, 0.0f, (W_WIDTH / 60),
			  (W_HEIGHT / 30));
		}
	    }
	    w++;
	  }
	h++;
      }
  }

  void		GraphOpenGL::drawSection(IGraph::section_t const& section,
					 PointDraw const& pts,
					 ColorToDraw const, bool select)
  {
    int	posX;
    int	posY;
    int idx = 0;

    if (window == NULL)
      drawWindow();
    initDrawMap();
    glColor3f(0.0f, 0.0f, 1.0f);
    glViewport((pts.posX * (W_WIDTH / 60)), (W_HEIGHT - (pts.height * (W_HEIGHT / 30)))
	       - ((pts.posY) * (W_HEIGHT / 30)), W_HEIGHT, W_WIDTH);
    glRectf(0.0f, 0.0f, pts.width * (W_WIDTH / 60),
	    pts.height * (W_HEIGHT / 30));
    for (std::size_t it = 0; it != section.first.size(); ++it)
      {
	glViewport(0, -30, W_HEIGHT, W_WIDTH);
	posX = ((pts.posX + 1) * (W_WIDTH / 60));
	posY = (idx * ((pts.height * (W_HEIGHT / 30)) / section.first.size()))
	  + ((pts.posY + 1) * (W_HEIGHT / 30));
	posY = W_HEIGHT - posY;
	if (it == section.second)
	  renderBitmapString(posX, posY, true, section.first[it]);
	else
	  renderBitmapString(posX, posY, false, section.first[it]);
	idx++;
	drawEdges(pts, select);
      }
    SDL_GL_SwapBuffers();
  }

  void		GraphOpenGL::drawSection(std::string const& section,
					 PointDraw const& pts,
					 ColorToDraw const color, bool select)
  {
    std::vector<std::string> sections;

    sections.push_back(section);
    drawSection(std::make_pair(sections, 1), pts, color, select);
  }

  void		GraphOpenGL::drawPlayer(WhereAmI const *character,
					GetMap const *map,
					PointDraw const&pts)
  {
    int	posX;
    int	posY;
    int w = (W_WIDTH / map->width) * pts.posX;
    int	h = (W_HEIGHT / map->height) * pts.posY;
    float limitx;
    float limity;

    limitx = (W_WIDTH - (pts.posX * (W_WIDTH / map->width))) / map->width;
    limity = (W_HEIGHT - (pts.posY * (W_HEIGHT / map->height))) / map->height;
    for (uint16_t idx = 0; idx < character->lenght; idx++)
      {
	posX = (character->position[idx].x *
		((W_WIDTH - w) / map->width)) +
	  (pts.posX * (W_WIDTH / map->width));
	posY = (character->position[idx].y *
		((W_HEIGHT - h) / map->height)) +
	  ((pts.posY + 1) * (W_HEIGHT / map->height));
	posY = W_HEIGHT - posY;
	glViewport(posX, posY, W_HEIGHT, W_WIDTH);
	if (idx == 0)
	  {
	    glColor3f(0.0f, 0.0f, 1.0f);
	    glRectf(0.0f, 0.0f, limitx, limity);
	  }
	else
	  {
	    glColor3f(0.0f, 1.0f, 1.0f);
	    glRectf(0.0f, 0.0f, limitx, limity);
	  }
      }
  }

  void		GraphOpenGL::drawType(arcade::TileType type, float limitx, float limity) const
  {
    if (type == arcade::TileType::EVIL_DUDE)
      {
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(0.0f, 0.0f, limitx, limity);
      }
    if (type == arcade::TileType::POWERUP)
      {
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(0.0f, 0.0f, limitx, limity);
      }
    if (type == arcade::TileType::BLOCK)
      {
	glColor3f(0.5f, 0.5f, 0.5f);
	glRectf(0.0f, 0.0f, limitx, limity);
      }
    if (type == arcade::TileType::MY_SHOOT)
      {
	glColor3f(1.0f, 0.5f, 0.0f);
	glRectf(0.0f, 0.0f, limitx, limity);
      }
    if (type == arcade::TileType::OBSTACLE)
      {
	glColor3f(0.58f, 0.26f, 0.79f);
	glRectf(0.0f, 0.0f, limitx, limity);
      }
  }

  void		GraphOpenGL::drawMap(GetMap const *map,
				     WhereAmI const *character,
				     PointDraw const&pts)
  {
    int	posX;
    int posY;
    float limitx;
    float limity;

    if (window == NULL)
      drawWindow();
    glClear(GL_COLOR_BUFFER_BIT);
    initDrawMap();
    limitx = (W_WIDTH - (pts.posX * (W_WIDTH / map->width))) / map->width;
    limity = (W_HEIGHT - (pts.posY * (W_HEIGHT / map->height))) / map->height;
    for (uint16_t y = 0; y < map->height; ++y){
      for (uint16_t x = 0; x < map->width; ++x){
	posX = (x * ((W_WIDTH - ((W_WIDTH / map->width) * pts.posX)) / map->width))
	  + (pts.posX * (W_WIDTH / map->width));
	posY = (y * ((W_HEIGHT - ((W_HEIGHT / map->height) * pts.posY)) / map->height))
	  + ((pts.posY + 1) * (W_HEIGHT / map->height));
	posY = W_HEIGHT - posY;
	glViewport(posX, posY, W_HEIGHT, W_WIDTH);
	drawType(map->tile[y * map->width + x], limitx, limity);
      }
    }
    drawPlayer(character, map, pts);
  }

  void	GraphOpenGL::stop(){
    _isRunning = false;
  }

  void	GraphOpenGL::loop(GraphOpenGL::update_t const& func,
			  GraphOpenGL::broadCmd_t const& broadCmd)
  {
    _isRunning = true;
    SDL_EnableKeyRepeat(1, 1500);
    while (_isRunning)
      {
	if (keyHook(broadCmd) == -1)
	  return ;
	func();
	AGraph::unstackQueueEvent(broadCmd);
	SDL_Delay(60);
      }
  }

  void	GraphOpenGL::setPath(std::string const&){
    return ;
  }
}

extern "C" {
  arcade::IGraph	*createGraph(std::string const& path){
    return new arcade::GraphOpenGL{ path };
  }
}

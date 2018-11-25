#include <iostream>
#include <sstream>
#include "keyEventError.hh"
#include "EventHandler.hh"
#include "GraphException.hh"
#include "GraphSDL.hpp"

namespace arcade {
  GraphSDL::		GraphSDL(std::string const&)
  {
    _isRunning = true;
    window = NULL;
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

    if (W_WIDTH <= 0 || W_HEIGHT <= 0 || W_WIDTH > 1000 || W_HEIGHT > 1000)
      throw GraphError {"Window too big", "GraphSDL"};
    if (TTF_Init() == -1)
      throw GraphError {"Can't init TTF", "GraphSDL"};
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
      throw GraphError {"Can't init SDL", "GraphSDL"};
    if ((Fruit = IMG_Load("./graph/assets/fruit.png")) == NULL)
      throw GraphError {"Can't Load IMG", "GraphSDL"};
    if ((Block = IMG_Load("./graph/assets/block.png")) == NULL)
      throw GraphError {"Can't Load IMG", "GraphSDL"};
    if ((Evil_dude = IMG_Load("./graph/assets/evil_dude.png")) == NULL)
      throw GraphError {"Can't Load IMG", "GraphSDL"};
    if ((My_shoot = IMG_Load("./graph/assets/my_shoot.png")) == NULL)
      throw GraphError {"Can't Load IMG", "GraphSDL"};
    if ((Head = IMG_Load("./graph/assets/head.png")) == NULL)
      throw GraphError {"Can't Load IMG", "GraphSDL"};
    if ((Body = IMG_Load("./graph/assets/body.png")) == NULL)
      throw GraphError {"Can't Load IMG", "GraphSDL"};
    if ((Obstacle = IMG_Load("./graph/assets/obstacle.png")) == NULL)
      throw GraphError {"Can't Load IMG", "GraphSDL"};
    SDL_WM_SetCaption("Wololo", NULL);
    drawWindow();
  }

  GraphSDL::~GraphSDL()
  {
    SDL_FreeSurface(Fruit);
    SDL_FreeSurface(Block);
    SDL_FreeSurface(Evil_dude);
    SDL_FreeSurface(Head);
    SDL_FreeSurface(Body);
    SDL_FreeSurface(Obstacle);
    TTF_Quit();
    SDL_Quit();
  }

  void		GraphSDL::drawPixel(const SDL_Surface *surface, int x, int y,
				    Uint32 color) const
  {
    Uint8	*pixel;

    pixel = reinterpret_cast<Uint8 *>(surface->pixels);
    pixel += (y * surface->pitch) + (x * surface->format->BytesPerPixel);
    *reinterpret_cast<Uint32 *>(pixel) = color;
  }

  SDL_Surface	*GraphSDL::drawColoredBlock(Uint32 color, int width, int height) const
  {
    SDL_Surface *Surface;
    int		x = 0;
    int		y = 0;

    Surface = SDL_CreateRGBSurface(0, width,
				   height, 32, 0, 0, 0, 0);
    if (Surface == NULL)
      throw GraphError {"Can't create RGBSurface", "drawColoredBlock"};
    while (y < height)
      {
	while (x < width)
	  {
	    drawPixel(Surface, x, y, color);
	    x++;
	  }
	x = 0;
	y++;
      }
    return (Surface);
  }

  void		GraphSDL::drawWindow()
  {
    if ((window = SDL_SetVideoMode(W_WIDTH, W_HEIGHT, W_BPP, SDL_HWSURFACE)) == NULL)
      throw GraphError {"Can't set VideoMode", "drawWindow"};
  }

  void			GraphSDL::drawEdges(PointDraw const& pts, bool select) const
  {
    unsigned int	h = 0;
    unsigned int	w = 0;
    SDL_Surface		*block;
    SDL_Surface		*block_s;
    SDL_Rect		pos;

    block = drawColoredBlock(0x00FFFF, (W_WIDTH / 60),
			     (W_HEIGHT / 30));
    block_s = drawColoredBlock(0xFFFF00, (W_WIDTH / 60),
			       (W_HEIGHT / 30));
    while (h < pts.height)
      {
	w = 0;
	while (w < pts.width)
	  {
	    pos.x = ((pts.posX + w) * (W_WIDTH / 60));
	    pos.y = ((pts.posY + h) * (W_HEIGHT / 30));
	    if ((h == 0 || h == pts.height -1) || (w == 0 || w == pts.width -1
						   || w == pts.width - 2)){
	      if (!select)
		SDL_BlitSurface(block, NULL, window, &pos);
	      else
		SDL_BlitSurface(block_s, NULL, window, &pos);
	    }
	    w++;
	  }
	h++;
      }
  }

  void		GraphSDL::drawSection(IGraph::section_t const& section,
				      PointDraw const& pts,
				      ColorToDraw const, bool select)
  {
    int		idx = 0;
    SDL_Surface	*text;
    SDL_Color	color;
    TTF_Font   *policy;
    SDL_Rect	pos;
    SDL_Surface *Font;

    if (window == NULL)
      drawWindow();
    Font = drawColoredBlock(0x0000FF, pts.width * (W_WIDTH / 60),
			    pts.height * (W_HEIGHT / 30));
    pos.x = (pts.posX * (W_WIDTH / 60));
    pos.y = (pts.posY * (W_HEIGHT / 30));
    SDL_BlitSurface(Font, NULL, window, &pos);
    if ((policy = TTF_OpenFont("./graph/assets/font.otf", 24)) == NULL)
      throw GraphError {"Can't LOAD TTF font", "drawSection"};
    for (std::size_t it = 0; it != section.first.size(); ++it)
      {
	pos.x = ((pts.posX + 1) * (W_WIDTH / 60));
	pos.y = (idx * ((pts.height * (W_HEIGHT / 30)) / section.first.size()))
	  + ((pts.posY + 1) * (W_HEIGHT / 30));
	if (it == section.second)
	  {
	    color = {255, 0, 255, 255};
	    text = TTF_RenderText_Blended(policy, section.first[it].c_str(), color);
	  }
	else
	  {
	    color = {255, 255, 255, 255};
	    text = TTF_RenderText_Blended(policy, section.first[it].c_str(), color);
	  }
	drawEdges(pts, select);
	SDL_BlitSurface(text, NULL, window, &pos);
	idx++;
      }
    SDL_Flip(window);
    TTF_CloseFont(policy);
  }

  void			GraphSDL::drawSection(std::string const& section,
					      PointDraw const& pts,
					      ColorToDraw const color, bool select)
  {
    std::vector<std::string> sections;

    sections.push_back(section);
    drawSection(std::make_pair(sections, 1), pts, color, select);
  }

  void		GraphSDL::drawPlayer(WhereAmI const *character,
				     GetMap const *map,
				     SDL_Rect const *limit,
				     PointDraw const& pts)
  {
    SDL_Rect	pos;
    int		limitx = (W_WIDTH / map->width) * pts.posX;
    int		limity = (W_HEIGHT / map->height) * pts.posY;

    for (uint16_t idx = 0; idx < character->lenght; ++idx)
      {
	pos.x = (character->position[idx].x *
		 ((W_WIDTH - limitx) / map->width)) +
	  (pts.posX * (W_WIDTH / map->width));
	pos.y = (character->position[idx].y *
		 ((W_HEIGHT - limity) / map->height)) +
	  (pts.posY * (W_HEIGHT / map->height));
	if (idx == 0)
	  SDL_BlitSurface(Head, const_cast<SDL_Rect *>(limit), window, &pos);
	else
	  SDL_BlitSurface(Body, const_cast<SDL_Rect *>(limit), window, &pos);
      }
  }


  void		GraphSDL::drawMap(GetMap const *map,
				  WhereAmI const *character,
				  PointDraw const&pts)
  {
    SDL_Surface *BackGround;
    SDL_Rect	pos;
    SDL_Rect	limit;


    limit.x = 0;
    limit.y = 0;
    BackGround = drawColoredBlock(0x000000, W_WIDTH, W_HEIGHT);
    pos.x = (pts.posX * (W_WIDTH / map->width));
    pos.y = (pts.posY * (W_HEIGHT / map->height));
    limit.w = (W_WIDTH - pos.x) / map->width;
    limit.h = (W_HEIGHT - pos.y) / map->height;
    SDL_BlitSurface(BackGround, NULL, window, &pos);
    for (uint16_t y = 0; y < map->height; ++y){
      for (uint16_t x = 0; x < map->width; x++) {

	pos.x = (x * ((W_WIDTH - ((W_WIDTH / map->width) * pts.posX)) / map->width))
	  + (pts.posX * (W_WIDTH / map->width));
	pos.y = (y * ((W_HEIGHT - ((W_HEIGHT / map->height) * pts.posY)) / map->height))
	  + (pts.posY * (W_HEIGHT / map->height));
    	if (map->tile[y * map->width + x] == arcade::TileType::POWERUP)
	  SDL_BlitSurface(Fruit, &limit, window, &pos);
	if (map->tile[y * map->width + x] == arcade::TileType::BLOCK)
	  SDL_BlitSurface(Block, &limit, window, &pos);
	if (map->tile[y * map->width + x] == arcade::TileType::EVIL_DUDE)
	  SDL_BlitSurface(Evil_dude, &limit, window, &pos);
	if (map->tile[y * map->width + x] == arcade::TileType::MY_SHOOT)
	  SDL_BlitSurface(My_shoot, &limit, window, &pos);
	if (map->tile[y * map->width + x] == arcade::TileType::OBSTACLE)
	  SDL_BlitSurface(Obstacle, &limit, window, &pos);
      }
    }
    SDL_FreeSurface(BackGround);
    drawPlayer(character, map, &limit, pts);
    SDL_Flip(window);
  }

  void	GraphSDL::stop() {
    _isRunning = false;
  }

  void	GraphSDL::loop(GraphSDL::update_t const& func,
		       GraphSDL::broadCmd_t const& broadCmd)
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

  void			GraphSDL::setPath(std::string const&) {
    return ;
  }
}

extern "C" {
  arcade::IGraph	*createGraph(std::string const& path) {
    return new arcade::GraphSDL{ path };
  }
}

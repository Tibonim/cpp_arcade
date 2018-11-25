#include <unistd.h>
#include <iostream>
#include "keyEventError.hh"
#include "EventHandler.hh"
#include <ncurses/curses.h>
#include "GraphException.hh"
#include "GraphNcurses.hh"

namespace	arcade {
  GraphNcurses::GraphNcurses(std::string const&)
    : _map{ 0 } {
      initscr();
      if (has_colors())
	start_color();
      cbreak();
      noecho();
      keypad(stdscr, TRUE);
      refresh();
      curs_set(0);
      _cmdToPush[KEY_UP] = &AGraph::pushKeyUp;
      _cmdToPush[KEY_DOWN] = &AGraph::pushKeyDown;
      _cmdToPush[KEY_RIGHT] = &AGraph::pushKeyRight;
      _cmdToPush[KEY_LEFT] = &AGraph::pushKeyLeft;
      _cmdToPush[' '] = &AGraph::pushShoot;
      _cmdToPush['\033'] = &AGraph::pushQuit;
      _cmdToPush[KEY_BACKSPACE] = &AGraph::pushDeletion;
      _typeToDraw[TileType::BLOCK] = &GraphNcurses::drawBlock;
      _typeToDraw[TileType::OBSTACLE] = &GraphNcurses::drawObstacle;
      _typeToDraw[TileType::EVIL_DUDE] = &GraphNcurses::drawEvilDude;
      _typeToDraw[TileType::EVIL_SHOOT] = &GraphNcurses::drawEvilShoot;
      _typeToDraw[TileType::MY_SHOOT] = &GraphNcurses::drawMyShoot;
      _typeToDraw[TileType::POWERUP] = &GraphNcurses::drawPowerup;
      _typeToDraw[TileType::OTHER] = &GraphNcurses::drawOther;
      _typeToDraw[TileType::EMPTY] = &GraphNcurses::drawEmpty;
      _select = false;
  }

  GraphNcurses::~GraphNcurses() {
    destroyAllWindow();
    endwin();
  }

  void		GraphNcurses::stop() {
    AGraph::_isRunning = false;
  }

  void		GraphNcurses::stackInQueue(int ch) const {
    if (isInMap(ch))
      push(ch);
    else
      pushAscii(ch);
  }

  void		GraphNcurses::push(int ch) const {
    auto const	&it = _cmdToPush.find(ch);
    if (it == _cmdToPush.end())
      throw keyEventError{ "Error: event not found", "push" };
    (this->*it->second)();
  }

  bool		GraphNcurses::isInMap(int ch) const {
    return (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT ||
	    ch == KEY_RIGHT || ch == ' ' || ch == '\033' ||
	    ch == KEY_BACKSPACE);
  }

  int		GraphNcurses::keyHook(GraphNcurses::broadCmd_t
				      const&) {
    int		ch{ 0 };
    timeout(35);
    ch = getch();
    if (ch != -1)
      stackInQueue(ch);
    return 0;
  }

  void		GraphNcurses::drawWindow() {
    return ;
  }

  void		GraphNcurses::drawSection(IGraph::section_t const& section,
					  PointDraw const& pos,
					  ColorToDraw const, bool select) {
    _select = select;
    WINDOW	*_win = createWindow(pos.height, pos.width, pos.posY,
				     pos.posX);
    std::size_t	posX { 3 };
    std::size_t	posY { pos.height / 2 };
    if (pos.height / 2 + section.first.size() >= pos.height)
      posY = 3;
    for (std::size_t it = 0;
	 it != section.first.size(); ++it) {
      if (it != section.second)
	mvwaddstr(_win, posY, posX, section.first[it].c_str());
      else {
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	wattron(_win, COLOR_PAIR(6) | A_UNDERLINE);
	mvwaddstr(_win, posY, posX, section.first[it].c_str());
	wattroff(_win, COLOR_PAIR(6) | A_UNDERLINE);
      }
      ++posY;
    }
    _window.push_back(_win);
    wrefresh(_win);
    refresh();
    _select = false;
  }

  void		GraphNcurses::drawSection(std::string const& section,
					  PointDraw const& pos,
					  ColorToDraw const c,
					  bool select) {
    std::vector<std::string>	contain{ section };
    drawSection(std::make_pair(contain, 1), pos, c, select);
  }

  void		GraphNcurses::loop(GraphNcurses::update_t const& func,
				   AGraph::broadCmd_t const& broadCmd) {
    std::size_t	i{ 0 };
    AGraph::_isRunning = true;

    while (AGraph::_isRunning) {
      if (keyHook(broadCmd) == -1)
	return ;
      AGraph::unstackQueueEvent(broadCmd);
      if (i == 4) {
	func();
	i = 0;
      }
      ++i;
    }
    destroyAllWindow();
    endwin();
    return ;
  }

  void		GraphNcurses::drawLandscape(GetMap const *map) const {
    for (uint16_t idx = 0; idx < map->height; ++idx) {
      for (uint16_t col = 0; col < map->width; ++col) {
	uint16_t type{ static_cast<uint16_t>(map->tile[idx *
						       map->width + col]) };
	auto const& it = _typeToDraw.find(map->tile[idx * map->width + col]);
	if (it == _typeToDraw.end())
	  throw GraphError{ "Error: unknow type " +
	      std::to_string(type), "draw" };
	(this->*it->second)(idx, col);
      }
    }
  }

  void		GraphNcurses::drawCharacter(WhereAmI const *character) const {
    for (uint16_t idx = 0; idx < character->lenght; ++idx) {
      uint16_t	x{ character->position[idx].x };
      uint16_t	y{ character->position[idx].y };
      if (idx == 0) {
	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(_map, COLOR_PAIR(1) | A_BOLD);
	mvwaddstr(_map, y, x, "O");
	wattroff(_map, COLOR_PAIR(1) | A_BOLD);
      }
      else {
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	wattron(_map, COLOR_PAIR(6) | A_BOLD);
	mvwaddstr(_map, y, x, "s");
	wattroff(_map, COLOR_PAIR(6) | A_BOLD);
      }
    }
  }

  void		GraphNcurses::drawMap(GetMap const *map,
				      WhereAmI const *character,
				      PointDraw const& pos) {
    WINDOW	*win;

    if (_map)
      destroyWindow(_map);
    win = createWindow(map->height + 2, map->width + 2, pos.posY,
		       pos.posX);
    if (!(_map = newwin(map->height, map->width, pos.posY + 1,
			pos.posX + 1)))
      throw GraphError { "Error: Cannot create window", "createWindow" };
    if (wrefresh(win) == ERR)
      throw GraphError { "Error: Cannot refresh window", "createWindow" };
    drawLandscape(map);
    drawCharacter(character);
    wrefresh(_map);
    _window.push_back(win);
  }

  void		GraphNcurses::moveCursor(unsigned row, unsigned col,
					 std::string const &score) const {
    std::size_t	posY { row / 2 };
    std::size_t	posX { (col / 2) - (score.size() - 1) };
    move(posY, posX);
  }

  WINDOW	*GraphNcurses::createWindow(unsigned row, unsigned col,
					    unsigned posXinWindow,
					    unsigned posYinWindow) const {
    WINDOW	*win;

    if (_select)
      init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    if (!(win = newwin(row, col, posXinWindow, posYinWindow)))
      throw GraphError { "Error: Cannot create window", "createWindow" };
    if (_select)
      wattron(win, COLOR_PAIR(3));
    box(win, '*', '#');
    if (_select)
      wattroff(win, COLOR_PAIR(3));
    if (wrefresh(win) == ERR)
      throw GraphError { "Error: Cannot refresh window", "createWindow" };
    return win;
  }

  void		GraphNcurses::destroyAllWindow() {
    for (auto it : _window)
      destroyWindow(it);
    _window.clear();
    clear();
  }

  void		GraphNcurses::destroyWindow(WINDOW *localWin) const {
    wborder(localWin, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(localWin);
    delwin(localWin);
  }

  void		GraphNcurses::setPath(std::string const&) {
    return ;
  }
}
extern "C" {
  arcade::IGraph	*createGraph(std::string const& path) {
    return new arcade::GraphNcurses{ path };
  }
}

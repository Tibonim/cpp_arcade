#pragma once

#include <functional>
#include <map>
#include <ncurses/curses.h>
#include "Protocol.hpp"
#include "AGraph.hh"

namespace	arcade {
  class		GraphNcurses : public AGraph {
    typedef void (AGraph::*toPush_t) () const;
    typedef void (GraphNcurses::*toDraw_t) (uint16_t, uint16_t) const;
    typedef std::function<void()>		update_t;
  public:
    GraphNcurses(std::string const& path = "");
    virtual	~GraphNcurses();
    int		keyHook(broadCmd_t const& broadCmd) override;
    void	drawWindow() override;
    void	drawSection(std::string const& section,
			    PointDraw const& point,
			    ColorToDraw const color,
			    bool select) override;
    void	drawSection(IGraph::section_t const& section,
			    PointDraw const& position,
			    ColorToDraw const color, bool select) override;
    void	drawMap(GetMap const *map, WhereAmI const *character,
			PointDraw const& position) override;
    void	loop(update_t const&, AGraph::broadCmd_t const&) override;
    void	stop() override;
    void	setPath(std::string const& path) override;
  private:
    void	stackInQueue(int ch) const;
    void	push(int ch) const;
    void	destroyWindow(WINDOW *localWin) const;
    WINDOW	*createWindow(unsigned row, unsigned col,
			     unsigned posXinWindow,
			     unsigned posYinWindow) const;
    void	moveCursor(unsigned row,
			   unsigned col, std::string const&) const;
    void	drawLandscape(GetMap const *map) const;
    void	drawBlock(uint16_t row, uint16_t col) const;
    void	drawObstacle(uint16_t row, uint16_t col) const;
    void	drawEvilDude(uint16_t row, uint16_t col) const;
    void	drawEvilShoot(uint16_t row, uint16_t col) const;
    void	drawMyShoot(uint16_t row, uint16_t col) const;
    void	drawPowerup(uint16_t row, uint16_t col) const;
    void	drawOther(uint16_t row, uint16_t col) const;
    void	drawEmpty(uint16_t row, uint16_t col) const;
    void	drawCharacter(WhereAmI const *character) const;
    void	destroyAllWindow();
    bool	isInMap(int ch) const;
  private:
    std::map<int, toPush_t>		_cmdToPush;
    std::map<TileType, toDraw_t>	_typeToDraw;
    std::vector<WINDOW *>		_window;
    WINDOW				*_map;
    bool				_select;
  };
}

extern "C" {
  arcade::IGraph	*createGraph(std::string const& path);
}

#pragma once

#include <map>
#include <string>
#include "IGraph.hpp"

namespace	arcade {
  class		scoreDisplayer {
    typedef std::map<int, std::string>		container_t;
    typedef std::vector<std::string>		section_t;
  public:
    scoreDisplayer(std::string const& game, IGraph *_graph);
    void	run();
  private:
    void	broadCmd(KeyEvent e, char c);
    void	update();
    void	feedPlayer();
    void	createOut() const;
    void	enter();
    void	drawFinal(PointDraw& point);
  private:
    std::string	_game;
    IGraph	*_graph;
    container_t	_players;
  };
}

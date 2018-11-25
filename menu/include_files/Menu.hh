#pragma once

#include <map>
#include "IGraph.hpp"
#include "Dir.hh"

namespace	arcade {
  class		Menu {
    typedef	std::vector<std::string>		container_t;
    typedef	std::pair<container_t, std::size_t>	pair_t;
    typedef void	(Menu::*action_t) ();
  public:
    Menu(IGraph *graph);
    void	run(std::string& game,
		    std::string& playerName);
    void	loadPtr(IGraph *);
  private:
    void	broadCmd(KeyEvent event, char c);
    void	feedGame();
    void	displaySection();
    void	moveUpIterator();
    void	moveDownIterator();
    void	displayNameEnter();
    void	stopLoop();
    void	displayCharacter(char c);
    bool	isFind(KeyEvent e);
    void	deleteCharacter();
  private:
    IGraph				*_graph;
    std::vector<std::string>		_game;
    pair_t				_selection;
    bool				_register;
    std::vector<std::string>		_playerName;
    std::map<KeyEvent, action_t>	_action;
    bool				_haveToPrintName;
  };
}

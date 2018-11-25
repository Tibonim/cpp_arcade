#pragma once

#include <iostream>
#include <map>
#include "IGraph.hpp"

namespace	arcade {
  class		Pause {
    typedef	std::vector<std::string>		container_t;
    typedef	std::pair<container_t, std::size_t>	pair_t;
    typedef void	(Pause::*action_t) ();
    typedef	std::map<KeyEvent, action_t>		makeAction_t;
  public:
    Pause(IGraph *graph);
    char			run();
    void			loadPtr(IGraph *graph);
  private:
    void			drawPauseMenu();
    void			update();
    void			broadCmd(KeyEvent e, char c);
    void			doAction(KeyEvent e);
    void			upSection();
    void			downSection();
    void			quitPause();
  private:
    IGraph			*_graph;
    makeAction_t		_action;
    std::size_t			_section;
    std::map<int, bool>		_value;
    std::map<int, char>		_selectEvent;
  };
}

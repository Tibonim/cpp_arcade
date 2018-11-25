#pragma once

#include "IGraph.hpp"
#include <utility>
#include <queue>

namespace	arcade {
  class		EventHandler {
    typedef std::pair<KeyEvent, char>	keyCode_t;
  public:
    static void		push(keyCode_t const& code);
    static keyCode_t	getLastElem();
    static void		clean();
    static bool		isEmpty();
  private:
    static std::queue<keyCode_t>	_event;
  };
}

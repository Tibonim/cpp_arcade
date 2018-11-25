#include "Pause.hh"

namespace	arcade {
  Pause::Pause(IGraph *graph) : _graph{ graph } {
    _action[K_UP] = &Pause::downSection;
    _action[K_DOWN] = &Pause::upSection;
    _action[QUIT] = &Pause::quitPause;
    _value[0] = false;
    _value[1] = false;
    _value[2] = false;
    _section = 0;
    _value[_section] = true;
    _selectEvent[0] = '\0';
    _selectEvent[1] = '8';
    _selectEvent[2] = '0';
  }

  void	Pause::drawPauseMenu() {
    Pause::container_t	keepGoing{ "CONTINUE" };
    Pause::container_t	restart{ "RESTART GAME" };
    Pause::container_t	quit{ "QUIT" };
    PointDraw		keeGoingP{ 15, 10, 5, 30 };
    PointDraw		restartP{ 15, 15, 5, 30 };
    PointDraw		quitP{ 15, 20, 5, 30 };
    _graph->drawSection(std::make_pair(keepGoing, 1), keeGoingP,
			ColorToDraw::RED, _value[0]);
    _graph->drawSection(std::make_pair(restart, 1), restartP,
			ColorToDraw::RED, _value[1]);
    _graph->drawSection(std::make_pair(quit, 1), quitP,
			ColorToDraw::RED, _value[2]);
  }

  void	Pause::upSection() {
    _value[_section] = false;
    if (_section == 2)
      _section = 0;
    else
      ++_section;
    _value[_section] = true;
  }

  void	Pause::downSection() {
    _value[_section] = false;
    if (_section == 0)
      _section = 2;
    else
      --_section;
    _value[_section] = true;
  }

  void	Pause::quitPause() {
    _graph->stop();
  }

  void	Pause::doAction(KeyEvent e) {
    auto const	&it = _action.find(e);
    if (it == _action.end())
      return ;
    (this->*it->second)();
  }

  void	Pause::update() {
    drawPauseMenu();
  }

  void	Pause::loadPtr(IGraph *graph) {
    _graph = graph;
  }

  void	Pause::broadCmd(KeyEvent e, char c) {
    if (!c)
      doAction(e);
    if (c == '\n')
      _graph->stop();
  }

  char	Pause::run() {
    drawPauseMenu();
    _graph->loop([this](){ return this->update(); },
		 [this](KeyEvent e, char c){ return this->broadCmd(e, c); });
    return _selectEvent[_section];
  }
}

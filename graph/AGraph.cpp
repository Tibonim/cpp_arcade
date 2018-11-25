#include "EventHandler.hh"
#include "keyEventError.hh"
#include "AGraph.hh"

namespace	arcade {
  AGraph::AGraph() : _isRunning{ false } {
  }

  void		AGraph::pushKeyUp() const {
    EventHandler::push(std::make_pair(KeyEvent::K_UP, '\0'));
  }

  void		AGraph::pushDeletion() const {
    EventHandler::push(std::make_pair(KeyEvent::DELETE, '\0'));
  }

  void		AGraph::pushKeyDown() const {
    EventHandler::push(std::make_pair(KeyEvent::K_DOWN, '\0'));
  }

  void		AGraph::pushKeyLeft() const {
    EventHandler::push(std::make_pair(KeyEvent::K_LEFT, '\0'));
  }

  void		AGraph::pushKeyRight() const {
    EventHandler::push(std::make_pair(KeyEvent::K_RIGHT, '\0'));
  }

  void		AGraph::pushQuit() const {
    EventHandler::push(std::make_pair(KeyEvent::QUIT, '\0'));
  }

  void		AGraph::pushShoot() const {
    EventHandler::push(std::make_pair(KeyEvent::SHOOT, '\0'));
  }

  void		AGraph::pushAscii(int ch) const {
    EventHandler::push(std::make_pair(KeyEvent::ASCII,
				      static_cast<char>(ch)));
  }

  void		AGraph::unstackQueueEvent(AGraph::broadCmd_t
					  const& broad) const {
    std::pair<KeyEvent, char>	cmd;

    while (!EventHandler::isEmpty()) {
      cmd = EventHandler::getLastElem();
      broad(cmd.first, cmd.second);
    }
  }
}

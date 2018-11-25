#include "EventHandler.hh"
#include <iostream>
namespace	arcade {

  std::queue<EventHandler::keyCode_t> EventHandler::_event;

  void	EventHandler::push(EventHandler::keyCode_t const &type)
  {
    while (_event.size() > 5)
      _event.pop();
    _event.push(type);
  }

  bool	EventHandler::isEmpty() {
    return !_event.size();
  }

  EventHandler::keyCode_t	EventHandler::getLastElem()
  {
    keyCode_t	ret{};

    ret = _event.front();
    _event.pop();
    return (ret);
  }

  void		EventHandler::clean()
  {
    std::queue<EventHandler::keyCode_t> empty;
    std::swap(_event, empty);
  }
}

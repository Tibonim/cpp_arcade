#include <iostream>
#include <algorithm>
#include <sstream>
#include "keyEventError.hh"
#include "EventHandler.hh"
#include "GraphException.hh"
#include "GraphSDL.hpp"

namespace arcade {

  void	GraphSDL::push(int ch) const {
    auto const &it = _cmdToPush.find(ch);
    if (it == _cmdToPush.end())
      throw keyEventError{ "Error: event not found", "push" };
    (this->*it->second)();
  }

  int	GraphSDL::getShiftKey(int ch) const {
    auto const &it = _shift_code.find(ch);
    if (it == _shift_code.end())
      return (-1);
    return ((*it).second);
  }

  void	GraphSDL::pushMyEvents() const
  {
    auto it = _key_event.begin();

    while (it != _key_event.end())
      {
	push(*it);
	it++;
      }
  }

  void	GraphSDL::unstackKeyEvent(int event)
  {
    _key_event.erase(std::remove(_key_event.begin(), _key_event.end(), event),
		     _key_event.end());
  }

  void	GraphSDL::pushKeyEvent(int event)
  {
    auto it = _key_event.begin();

    if (event == SDLK_UP
	|| event == SDLK_DOWN
	|| event == SDLK_LEFT
	|| event == SDLK_RIGHT
	|| event == SDLK_SPACE
	|| event == SDLK_BACKSPACE){
      while (it != _key_event.end())
	{
	  if (*it == event)
	    return ;
	  it++;
	}
      _key_event.push_back(event);
    }
  }

  int	GraphSDL::keyHook(GraphSDL::broadCmd_t const&)
  {
    int	nb = -1;

    SDL_PollEvent(&event);
    if (event.key.keysym.mod & KMOD_SHIFT && event.type == SDL_KEYDOWN)
      nb = getShiftKey(event.key.keysym.sym);
    if (nb != -1)
      EventHandler::push(std::make_pair(ASCII, nb));
    if (event.type == SDL_KEYUP)
      unstackKeyEvent(event.key.keysym.sym);
    if (event.type == SDL_KEYDOWN &&
	event.key.keysym.sym == SDLK_BACKSPACE)
      pushDeletion();
    else if (event.type == SDL_KEYDOWN
	&& event.key.keysym.sym != SDLK_UNKNOWN
	&& nb == -1)
      {
	if (event.type == SDL_QUIT ||
	    event.key.keysym.sym == SDLK_ESCAPE)
	  push(SDLK_ESCAPE);
	pushKeyEvent(event.key.keysym.sym);
	if (event.key.keysym.sym == SDLK_RETURN)
	  EventHandler::push(std::make_pair(ASCII, '\n'));
	if (event.key.keysym.sym != SDLK_LSHIFT)
	  EventHandler::push(std::make_pair(ASCII, event.key.keysym.sym));
      }
    pushMyEvents();
    event.key.keysym.sym = SDLK_UNKNOWN;
    return (0);
  }
}

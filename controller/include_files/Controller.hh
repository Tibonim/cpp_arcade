#pragma once

#include <map>
#include <string>
#include "EventHandler.hh"
#include "Menu.hh"
#include "IGame.hpp"
#include "Pause.hh"
namespace	arcade{ class IGraph; }
#include "IGraph.hpp"
#include "Loader.hpp"
#include "scoreDisplayer.hh"

namespace	arcade {
  class		Controller {
    typedef void	(Controller::*ptrChanging) ();
  public:
    Controller(std::string const&);
    ~Controller();
    void	run();
  private:
    void	update();
    void	broadCmd(KeyEvent const event, char c);
    void	feedLibrary(std::vector<std::string>& gameList,
			    std::string const& directory);
    bool	isNotAgame(std::string const& gameName) const;
    CommandType	convertInCmd(KeyEvent const event) const;
    void	try_launch(char c);
    void	startMenu();
    void	backMenu();
    void	startGame();
    void	restartGame();
    void	nextGfxLib();
    void	prevGfxLib();
    void	prevGame();
    void	nextGame();
    void	startPause();
    template<class InputIt, class T, class Loader>
    void	nextLib(InputIt it, InputIt end, T const& lib, Loader func,
			T const& concat);
    template<class InputIt, class T, class Loader>
    void	prevLib(InputIt it, InputIt begin, T const& lib, Loader func,
			T const& concat);
    void	loadGfx(std::string const& lib);
    void	loadGame(std::string const& lib);
    void	goNextLib();
    void	goPrevLib();
    void	goPrevGlib();
    void	goNextGlib();
  private:
    IGame					*_game;
    scoreDisplayer				*_scoreDisplayer;
    IGraph					*_graph;
    Menu					*_menu;
    Pause					*_pause;
    std::string					_gfxLib;
    std::vector<std::string>			_gameList;
    std::vector<std::string>			_gfxList;
    std::map<KeyEvent, CommandType> const	_event;
    std::string					_gameToPlay;
    std::string					_playerName;
    std::map<char, ptrChanging>			_changeInGame;
    char					_c;
    Loader<IGame>				_loaderGame;
    Loader<IGraph>				_loaderGraph;
    std::function<void()>			_inst;
    bool					_isAnInstruction;
  };
}

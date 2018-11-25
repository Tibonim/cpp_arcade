#include <iostream>
#include <algorithm>
#include "keyEventError.hh"
#include "Dir.hh"
#include "Loader.hpp"
#include "GameError.hh"
#include "Controller.hh"

namespace	arcade {
  Controller::Controller(std::string const& gfxLib)
    : _game{ 0 }, _scoreDisplayer{ 0 }, _graph{ 0 }, _menu{ 0 }, _pause{ 0 },
    _event{{KeyEvent::K_UP, CommandType::GO_UP},
	{KeyEvent::K_DOWN, CommandType::GO_DOWN},
	     {KeyEvent::K_LEFT, CommandType::GO_LEFT},
	     {KeyEvent::K_RIGHT, CommandType::GO_RIGHT},
	     {KeyEvent::SHOOT, CommandType::SHOOT}}
   {
     loadGfx(gfxLib);
     std::size_t	posGfx{ gfxLib.find("lib/") };
     if (posGfx == std::string::npos)
       _gfxLib = gfxLib;
     else {
       std::string	subGfx{ gfxLib.substr(posGfx + 4, gfxLib.size()) };
       _gfxLib = subGfx;
     }
      _menu = new Menu{ _graph };
      _pause = new Pause{ _graph };
      feedLibrary(_gameList, "./games/");
      feedLibrary(_gfxList, "./lib/");
      _changeInGame['2'] = &Controller::prevGfxLib;
      _changeInGame['3'] = &Controller::nextGfxLib;
      _changeInGame['4'] = &Controller::prevGame;
      _changeInGame['5'] = &Controller::nextGame;
      _changeInGame['8'] = &Controller::restartGame;
      _changeInGame['9'] = &Controller::backMenu;
      _changeInGame['p'] = &Controller::startPause;
      _c = 0;
      _isAnInstruction = false;
      _inst = [this](){ return this->_graph->stop(); };
      _graph->drawWindow();
   }

  Controller::~Controller() {
    if (_game) {
      delete _game;
      _loaderGame.close();
    }
    if (_graph) {
      delete _graph;
      _loaderGraph.close();
    }
    if (_menu)
      delete _menu;
    if (_pause)
      delete _pause;
  }

  void		Controller::startPause() {
    _c = _pause->run();
    if (!_c) {
      _isAnInstruction = true;
      _inst = [this]() { return this->_graph->stop(); };
      return ;
    }
    auto const&	it = _changeInGame.find(_c);
    if (it == _changeInGame.end())
      return ;
    (this->*it->second)();
  }

  void		Controller::feedLibrary(std::vector<std::string>& gameList,
					std::string const& dir) {
    Dir		directory{ dir };
    struct dirent	*dirent{ 0 };

    while ((dirent = directory.readDir())) {
      std::string fileName{ dirent->d_name };
      if (fileName.find("lib") == 0)
	gameList.push_back(fileName);
    }
  }

  bool		Controller::isNotAgame(std::string const& gameName) const {
    return std::find(_gameList.begin(),
		     _gameList.end(), gameName) == _gameList.end();
  }

  CommandType	Controller::convertInCmd(KeyEvent const event) const {
    auto const	&it = _event.find(event);
    if (it == _event.end())
      throw keyEventError{ "Error: event not recognized", "convertInCmd" };
    return it->second;
  }

  void		Controller::try_launch(char c) {
    auto const	&it = _changeInGame.find(c);
    if (it == _changeInGame.end())
      return ;
    (this->*it->second)();
    _c = 0;
  }

  void		Controller::broadCmd(KeyEvent const event,
				     char c) {
    CommandType	cmd;

    if (event == KeyEvent::QUIT)
      _graph->stop();
    else if (!c) {
      cmd = convertInCmd(event);
      if (!_game->sendCmd(cmd))
	_graph->stop();
    }
    else
      try_launch(c);
  }


  void		Controller::update() {
    std::string	score{ "Score: " + std::to_string(_game->getScore()) };
    std::vector<std::string>	scoring{ score };
    GetMap const		*map{ _game->getMap() };
    WhereAmI const		*wai{ _game->whereAmI() };
    std::size_t			width{ static_cast<std::size_t>(map->width) };
    PointDraw			point{ 0, 0, 3, width + 3 };
    PointDraw			pointMap{ 0, 3, 0, 0 };
    ColorToDraw			color{ ColorToDraw::RED };

    _graph->drawSection(std::make_pair(scoring, scoring.size()),
    			point, color);
    _graph->drawMap(map, wai, pointMap);
    if (!_game->update())
      _graph->stop();
  }

  void		Controller::loadGame(std::string const& lib) {
    if (_game) {
      delete _game;
      _loaderGame.close();
    }
    _game = 0;
    _loaderGame.open(lib);
    _game = _loaderGame.create("createGame");
    std::size_t	posGfx{ lib.find("games/") };
     if (posGfx == std::string::npos)
       _gameToPlay = lib;
     else {
       std::string	subGame{ lib.substr(posGfx + 6, lib.size()) };
       _gameToPlay = subGame;
     }
  }

  void		Controller::startMenu() {
    _menu->run(_gameToPlay, _playerName);
    if (isNotAgame(_gameToPlay))
      throw GameError{ "Error: " + _gameToPlay + std::string(" is not a game"),
	  "Controller::run" };
    loadGame("./games/" + _gameToPlay);
  }

  void		Controller::backMenu() {
    _graph->stop();
    _inst = [this](){ return this->startMenu(); };
    _isAnInstruction = true;
  }

  void		Controller::restartGame() {
    _game->restart();
    _inst = [this](){ return this->_graph->stop(); };
    _isAnInstruction = true;
  }

  void		Controller::startGame() {
    _graph->loop([this](){ this->update(); },
		 [this](KeyEvent const e, char c){ this->broadCmd(e, c); });
    if (_isAnInstruction) {
      _isAnInstruction = false;
      _inst();
      startGame();
    }
  }

  void		Controller::loadGfx(std::string const& lib) {
    if (_graph) {
      delete _graph;
      _loaderGraph.close();
    }
    _graph = 0;
    _loaderGraph.open(lib);
    _graph = _loaderGraph.create("createGraph", "");
     std::size_t	posGfx{ lib.find("lib/") };
     if (posGfx == std::string::npos)
       _gfxLib = lib;
     else {
       std::string	subGfx{ lib.substr(posGfx + 4, lib.size()) };
       _gfxLib = subGfx;
     }
    if (_menu)
      _menu->loadPtr(_graph);
    if (_pause)
      _pause->loadPtr(_graph);
  }

  void		Controller::nextGfxLib() {
    _graph->stop();
    _inst = [this](){ return this->goNextLib(); };
    _isAnInstruction = true;
  }

  void		Controller::prevGfxLib() {
    _graph->stop();
    _inst = [this](){ return this->goPrevLib(); };
    _isAnInstruction = true;
  }

  void		Controller::goNextLib() {
    std::string	path{ "./lib/" };
    nextLib(_gfxList.begin(), _gfxList.end(), _gfxLib,
    	    &Controller::loadGfx, path);
  }

  void		Controller::goPrevLib() {
    std::string	path{ "./lib/" };
    prevLib(_gfxList.begin(), _gfxList.end(), _gfxLib,
    	    &Controller::loadGfx, path);
  }

  void		Controller::goPrevGlib() {
    std::string	path{ "./games/" };
    prevLib(_gameList.begin(), _gameList.end(), _gameToPlay,
    	    &Controller::loadGame, path);
    update();
  }

  void		Controller::goNextGlib() {
    std::string	path{ "./games/" };
    nextLib(_gameList.begin(), _gameList.end(),
    	    _gameToPlay, &Controller::loadGame, path);
    update();
  }

  void		Controller::nextGame() {
    _graph->stop();
    _inst = [this](){ return this->goNextGlib(); };
    _isAnInstruction = true;
  }

  void		Controller::prevGame() {
    _graph->stop();
    _inst = [this](){ return this->goPrevGlib(); };
    _isAnInstruction = true;
  }

  template<class InputIt, class T, class Loader>
  void		Controller::nextLib(InputIt first, InputIt last,
				    T const& lib, Loader func, T const& concat) {
    auto	it = std::find(first, last, lib);
    if (it == last)
      it = first;
    else
      it++;
    if (it == last)
      it = first;
    (this->*func)(concat + (*it));
  }

  template<class InputIt, class T, class Loader>
  void		Controller::prevLib(InputIt first, InputIt last,
				    T const& lib, Loader func,
				    T const& concat) {
    auto	it = std::find(first, last, lib);
    if (it == first)
      it = last - 1;
    else
      it--;
    (this->*func)(concat + (*it));
  }

  void		Controller::run() {
    startMenu();
    _game->savePlayerName(_playerName);
    update();
    startGame();
    _scoreDisplayer = new scoreDisplayer{ "Snake", _graph };
    _scoreDisplayer->run();
  }
}

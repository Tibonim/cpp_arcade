#include <iostream>
#include "Menu.hh"

namespace	arcade {
  Menu::Menu(IGraph *graph) : _graph{ graph }, _register{ false },
    _haveToPrintName{ false } {
    _playerName.push_back("ENTER NAME : ");
    _action[KeyEvent::K_UP] = &Menu::moveDownIterator;
    _action[KeyEvent::K_DOWN] = &Menu::moveUpIterator;
    _action[KeyEvent::QUIT] = &Menu::stopLoop;
    feedGame();
  }

  void			Menu::feedGame() {
    Dir			gameDir{ "./games/" };
    struct dirent	*dirRead;
    while ((dirRead = gameDir.readDir())) {
      std::string	fileName{ dirRead->d_name };
      if (fileName.find("lib") == 0)
	_game.push_back(fileName);
    }
    _selection = std::make_pair(_game, 0);
  }

  void			Menu::loadPtr(IGraph *graph) {
    _graph = graph;
  }

  void			Menu::displaySection() {
    PointDraw		point{ 0, 0, _game.size() + 7, 40 };
    _graph->drawSection(_selection, point, ColorToDraw::RED);
  }

  void			Menu::moveUpIterator() {
    if (_selection.second + 1 == _selection.first.size()) {
      _selection.second = 0;
    }
    else
      ++_selection.second;
  }

  void			Menu::moveDownIterator() {
    if (_selection.second == 0)
      _selection.second = _selection.first.size() - 1;
    else
      --_selection.second;
  }

  void			Menu::displayCharacter(char c) {
    std::string		newString(1, c);
    if ((_playerName[0].size() - 13) < 10)
      _playerName[0] += newString;
  }

  void			Menu::stopLoop() {
    _graph->stop();
  }

  bool			Menu::isFind(KeyEvent e) {
    auto const		&it = _action.find(e);
    if (it == _action.end())
      return false;
    (this->*it->second)();
    return true;
  }

  void			Menu::deleteCharacter() {
    if (_playerName[0].size() > 13)
      _playerName[0].erase(_playerName[0].size() - 1);
  }

  void			Menu::broadCmd(KeyEvent event, char c) {
    if (c == '\n' || c == '\r') {
      _register = true;
      _graph->stop();
    }
    else if (isFind(event))
      return ;
    if (_haveToPrintName) {
      if (event == KeyEvent::DELETE || c == 8)
	deleteCharacter();
      else if (event == KeyEvent::ASCII && std::isalnum(c))
	displayCharacter(c);
    }
  }

  void			Menu::displayNameEnter() {
    PointDraw		point{ 0, 0, _game.size() + 7, 40 };
    _graph->drawSection(std::make_pair(_playerName, 1), point,
			ColorToDraw::RED);
  }

  void			Menu::run(std::string& game,
				  std::string& playerName) {
    displaySection();
    _graph->loop([this](){ this->displaySection(); },
		 [this](KeyEvent e, char c){ this->broadCmd(e, c);});
    if (_register)
      game = _game[_selection.second];
    _register = false;
    _haveToPrintName = true;
    displayNameEnter();
    _graph->loop([this](){ this->displayNameEnter(); },
    		 [this](KeyEvent e, char c){ this->broadCmd(e, c);});
    if (_register)
      playerName = _playerName[0].substr(13, _playerName[0].size() - 2);
  }

}

#include <bitset>
#include <iostream>
#include "AGame.hh"

namespace arcade {
  AGame::AGame(std::string const &game, uint16_t w, uint16_t h) : _playerName{ "" }
  {
    _tokens[CommandType::WHERE_AM_I] = &AGame::_whereAmI;
    _tokens[CommandType::GET_MAP] = &AGame::_getMap;
    _tokens[CommandType::GO_UP] = &AGame::_goUp;
    _tokens[CommandType::GO_DOWN] = &AGame::_goDown;
    _tokens[CommandType::GO_LEFT] = &AGame::_goLeft;
    _tokens[CommandType::GO_RIGHT] = &AGame::_goRight;
    _tokens[CommandType::GO_FORWARD] = &AGame::_goForward;
    _tokens[CommandType::SHOOT] = &AGame::_shoot;
    _tokens[CommandType::ILLEGAL] = &AGame::_illegal;
    _tokens[CommandType::PLAY] = &AGame::_play;
    _map = new GetMap[sizeof(TileType) * (w * h) + sizeof(GetMap)];
    _map->type = CommandType::GET_MAP;
    _map->width = w;
    _map->height = h;
    _game = game;
  }

  void	AGame::savePlayerName(std::string const& playerName) {
    _playerName = playerName;
  }

  AGame::~AGame()
  {
    delete [] _map;
  }

  const GetMap *AGame::getMap() const
  {
    return _map;
  }

  bool	AGame::_getMap()
  {
    const GetMap *map = getMap();
    std::cout.write(reinterpret_cast<const char*>(map),
		    sizeof(*map) +
		    (map->width * map->height) * sizeof(uint16_t));
    return true;
  }

  bool	AGame::_whereAmI()
  {
    const WhereAmI *wai = whereAmI();

    std::cout.write(reinterpret_cast<const char*>(wai),
		    sizeof(*wai) +
		    (wai->lenght) * sizeof(Position));
    delete wai;
    return true;
  }

  const WhereAmI *AGame::whereAmI() const
  {
    WhereAmI *wai;

    wai = new WhereAmI[_player.size() + sizeof(WhereAmI)];
    wai->type = CommandType::WHERE_AM_I;
    wai->lenght = _player.size();
    auto it = _player.begin();
    for (uint16_t i = 0; it != _player.end(); ++it, ++i)
      {
	wai->position[i].x = (*it)->x;
	wai->position[i].y = (*it)->y;
      }
    return (wai);
  }

  uint32_t	AGame::getScore() const
  {
    return _score;
  }

  bool	AGame::update()
  {
    if (_play() == true)
      return true;
    scoreWriter::writeScore(std::to_string(_score), _game, _playerName);
    return false;
  }

  uint32_t	AGame::getBestScore() const {
    return 0;
  }

  bool	AGame::sendCmd(CommandType cmd)
  {
    auto it = _tokens.find(cmd);
    if (it == _tokens.end())
      return true;
    if ((this->*(it->second))() == true)
      return true;
    scoreWriter::writeScore(std::to_string(_score), _game, _playerName);
    return false;
  }
}

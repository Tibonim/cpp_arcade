#include "Centipede.hh"

namespace arcade {
  Centipede::Centipede() : AGame("Centipede", 60, 30)
  {
    _initGame();
    _shootColide[TileType::OBSTACLE] = &Centipede::_shootObstacle;
    _shootColide[TileType::EVIL_DUDE] = &Centipede::_shootNode;
  }

  Centipede::~Centipede()
  {
  }

  void	Centipede::_generateNode()
  {
    if (_map->tile[_spawn] != TileType::EMPTY)
      _initSpawn();
    centipedeNode *node = new centipedeNode{_spawn, 0};
    _nodes.push_back(node);
    _map->tile[_spawn] = TileType::MY_SHOOT;
    ++_currLen;
  }

  void	Centipede::_shootObstacle(uint16_t i, uint16_t x, uint16_t y)
  {
    delete (_shoots[i]);
    _shoots.erase(_shoots.begin() + i);
    _destroyObstacle(y * _map->width + x);
  }

  void	Centipede::_destroyNode(uint16_t x, uint16_t y)
  {
    for (uint16_t i = 0; i < _nodes.size(); ++i)
      {
	if (_nodes[i]->getPos().x == x && _nodes[i]->getPos().y == y)
	  {
	    delete _nodes[i];
	    _nodes.erase(_nodes.begin() + i);
	    return ;
	  }
      }
  }

  void	Centipede::_shootNode(uint16_t i, uint16_t x, uint16_t y)
  {
    delete (_shoots[i]);
    _shoots.erase(_shoots.begin() + i);

    uint16_t pos = y * _map->width + x;
    _blocks[pos] = new Obstacle;
    _map->tile[pos] = TileType::OBSTACLE;
    _destroyNode(x, y);
    ++_score;
  }

  void	Centipede::_initGame()
  {
    _initMap();
    _limit = _map->height - (20 * _map->height / 100);
    _initPlayer();
    _initBlocks();
    _initSpawn();
    _score = 0;
    _cLen = 15;
    _currLen = 0;
  }

  void	Centipede::restart()
  {
    for (; _nodes.size();)
      {
	delete (_nodes.back());
	_nodes.pop_back();
      }
    _player.pop_back();
    _initGame();
  }

  void	Centipede::_initSpawn()
  {
    uint16_t pos;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _map->width - 1);
    do
    {
      pos = dis(gen);
    } while (_map->tile[pos] != TileType::EMPTY);
    _spawn = pos;
  }

  void	Centipede::_initPlayer()
  {
    Position	*pos = new Position;
    pos->x = _map->width / 2;
    pos->y = _limit + 1;
    _player.push_back(pos);
  }

  void	Centipede::_initMap()
  {
    for (uint16_t y = 0; y < _map->height; ++y)
      for (uint16_t x = 0; x < _map->width; ++x)
	_map->tile[y * _map->width + x] = TileType::EMPTY;
  }

  void	Centipede::_initBlocks()
  {
    Position *player = (_player.back());
    _blocks.clear();
    uint16_t max = _map->height * _map->width / 50;
    uint16_t pos;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _map->width * _map->height - 1);
    for (uint16_t i = 0; i < max; ++i)
      {
	do
	{
	  pos = dis(gen);
	} while (_map->tile[pos] != TileType::EMPTY &&
		 pos != player->y * _map->width + player->x);
	_blocks[pos] = new Obstacle;
	_map->tile[pos] = TileType::OBSTACLE;
      }
  }

  void	Centipede::_destroyObstacle(uint16_t pos)
  {
    if (_blocks[pos]->destroy())
      {
	delete _blocks[pos];
	_blocks.erase(pos);
	_map->tile[pos] = TileType::EMPTY;
      }
  }

  bool	Centipede::_canMove(uint16_t x, uint16_t y)
  {
    uint16_t pos = y * _map->width + x;
    if (_map->tile[pos] == TileType::EMPTY)
      return true;
    return false;
  }

  bool	Centipede::_alive(Position *pos)
  {
    for (auto it = _nodes.begin(); it != _nodes.end(); ++it)
      {
	if ((*it)->getPos().x == pos->x &&
	    (*it)->getPos().y == pos->y)
	  return (false);
      }
    return (true);
  }

  bool	Centipede::_tryMove(Position *pos, uint16_t x, uint16_t y)
  {
    if (y - 1 < _limit || !_canMove(x, y))
      return _alive(pos);
    if (y == _map->height || !_canMove(pos->x, pos->y))
      return _alive(pos);
    if (x != 1 && ((pos->x == 0 && x != pos->x) || !_canMove(x, pos->y)))
      return _alive(pos);
    if (x == _map->width || !_canMove(pos->x, pos->y))
      return _alive(pos);
    pos->x = x;
    pos->y = y;
    return _alive(pos);
  }

  bool	Centipede::_goUp()
  {
    Position *pos = (_player.back());

    return (_tryMove(pos, pos->x, pos->y - 1));
  }

  bool	Centipede::_goDown()
  {
    Position *pos = (_player.back());

    return (_tryMove(pos, pos->x, pos->y + 1));
  }

  bool	Centipede::_goLeft()
  {
    Position *pos = (_player.back());

    return _tryMove(pos, pos->x - 1, pos->y);
  }

  bool	Centipede::_goRight()
  {
    Position *pos = (_player.back());

    return _tryMove(pos, pos->x + 1, pos->y);
  }

  bool	Centipede::_goForward()
  {
    return true;
  }

  void	Centipede::_moveShoots()
  {
    for (uint16_t i = 0; i < _shoots.size(); ++i)
      {
	Position pos;
	pos.x = _shoots[i]->x;
	pos.y = _shoots[i]->y;
	_map->tile[pos.y * _map->width + pos.x] = TileType::EMPTY;
	if (pos.y == 0)
	  {
	    delete (_shoots[i]);
	    _shoots.erase(_shoots.begin() + i);
	    continue;
	  }
	--pos.y;
	auto it = _shootColide.find(_map->tile[pos.y * _map->width
					       + pos.x]);
	if (it != _shootColide.end())
	  {
	    (this->*it->second)(i, pos.x, pos.y);
	    continue;
	  }
	_shoots[i]->y = pos.y;
	_map->tile[pos.y * _map->width + pos.x] = TileType::MY_SHOOT;
      }
  }

  void	Centipede::_moveNodes()
  {
    for (uint16_t i = 0; i < _nodes.size(); ++i)
      {
	if (_nodes[i]->move(_map, &_score) == false)
	  {
	    uint16_t pos = _nodes[i]->getPos().x + _nodes[i]->getPos().y * _map->width;
	    _blocks[pos] = new Obstacle;
	    _map->tile[pos] = TileType::OBSTACLE;
	    delete _nodes[i];
	    _nodes.erase(_nodes.begin() + i);
	    ++_score;
	    --i;
	  }
      }
  }

  bool	Centipede::_shoot()
  {
    if (_shoots.size() >= 1)
      return (true);
    Position *pos = new Position;
    Position *player = (_player.back());

    pos->x = player->x;
    pos->y = player->y;
    _shoots.push_back(pos);
    return true;
  }

  bool	Centipede::_illegal()
  {
    return true;
  }

  bool	Centipede::_play()
  {
    if (_currLen < _cLen - 1)
      _generateNode();
    _moveNodes();
    _moveShoots();
    if (!_alive(_player.back()))
      return (false);
    if (_nodes.size() == 0)
      {
	_currLen = 0;
	_initSpawn();
      }
    return true;
  }
}

extern "C"
{
  arcade::IGame	*createGame()
  {
    return new arcade::Centipede;
  }

  void	Play(void)
  {
    static arcade::IGame *game = new arcade::Centipede;

    uint16_t test;

    while (std::cin.read(reinterpret_cast<char*>(&test), 2))
      game->sendCmd(static_cast<arcade::CommandType>(test));
    delete game;
  }
}

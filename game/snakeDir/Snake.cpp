#include "Snake.hh"

namespace arcade {
  Snake::Snake() : AGame("Snake", 60, 30)
  {
    _initGame();
  }

  Snake::~Snake()
  {
    clearSnake();
  }

  void Snake::restart()
  {
    clearSnake();
    _initGame();
  }

  void	Snake::_initGame()
  {
    _initMap();
    _initPlayer();
    _direction.x = 1;
    _direction.y = 0;
    _score = 0;
  }

  void	Snake::clearSnake()
  {
    for (auto it = _player.begin(); it != _player.end(); ++it)
      delete (*it);
    _player.clear();
  }

  void	Snake::_initPlayer()
  {
    for (int i = 0; i < 4; ++i)
      {
	Position *p = new Position;
	p->x = _map->width / 2 - i;
	p->y = _map->height / 2;
	_player.push_back(p);
      }
  }

  bool	Snake::_snakeIsPresent(uint16_t start, uint16_t x, uint16_t y)
  {
    auto it = _player.begin();
    for (uint16_t i = 0; i < start && it != _player.end(); ++i, ++it);
    for (; it != _player.end(); ++it)
      {
	if ((*it)->x == x && (*it)->y == y)
	  return (true);
      }
    return (false);
  }

  void Snake::_putFood()
  {
    uint16_t pos = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _map->width * _map->height - 1);
    do
    {
      pos = dis(gen);
    } while (_snakeIsPresent(0, pos % _map->width, pos / _map->width));
    _map->tile[pos] = TileType::POWERUP;
  }

  void	Snake::_initMap()
  {
    for (uint16_t x = 0; x < _map->width; ++x)
      {
	for (uint16_t y = 0; y < _map->height; ++y)
	  {
	    _map->tile[y * _map->width + x] = TileType::EMPTY;
	  }
      }
    _putFood();
  }

  bool	Snake::_willDie(int x, int y)
  {
    if (_snakeIsPresent(1, x, y))
      return (true);
    if (x + _direction.x < 0 || y + _direction.y < 0)
      return (true);
    if (x + _direction.x >= _map->width ||
	y + _direction.y >= _map->height)
      return (true);
    return (false);
  }

  void	Snake::_changeDirection()
  {
    Direction dir = _queueDirection.front();
    _queueDirection.pop();
    if ((dir == Direction::LEFT || dir == Direction::RIGHT) && _direction.x)
      return ;
    if ((dir == Direction::DOWN || dir == Direction::UP) && _direction.y)
      return ;
    _direction.x = 0;
    _direction.y = 0;
    if (dir == Direction::LEFT)
      _direction.x = -1;
    if (dir == Direction::RIGHT)
      _direction.x = 1;
    if (dir == Direction::UP)
      _direction.y = -1;
    if (dir == Direction::DOWN)
      _direction.y = 1;
  }

  bool	Snake::_move()
  {
    if (_queueDirection.size())
      _changeDirection();
    auto head = _player.begin();
    Position nextPos;
    nextPos.x = (*head)->x + _direction.x;
    nextPos.y = (*head)->y + _direction.y;
    if (_willDie((*head)->x, (*head)->y))
      {
    	clearSnake();
    	return false;
      }
    if (_map->tile[nextPos.y * _map->width + nextPos.x] == TileType::POWERUP)
      {
	/* EAT */
	Position *newNode = new Position;
	newNode->x = _player.back()->x;
	newNode->y = _player.back()->y;
	_player.push_back(newNode);
	_map->tile[nextPos.y * _map->width + nextPos.x] = TileType::EMPTY;
	++_score;
	_putFood();
      }
    auto currNode = _player.end();
    --currNode;
    while (currNode != _player.begin())
      {
	auto nextNode = currNode - 1;
	(*currNode)->x = (*nextNode)->x;
	(*currNode)->y = (*nextNode)->y;
	--currNode;
      }
    (*currNode)->x = nextPos.x;
    (*currNode)->y = nextPos.y;
    return true;
  }

  bool	Snake::_goUp()
  {
    if (_queueDirection.size() < 3)
      _queueDirection.push(Direction::UP);
    return true;
  }

  bool	Snake::_goDown()
  {
    if (_queueDirection.size() < 3)
      _queueDirection.push(Direction::DOWN);
    return true;
  }

  bool	Snake::_goLeft()
  {
    if (_queueDirection.size() < 3)
      _queueDirection.push(Direction::LEFT);
    return true;
  }

  bool	Snake::_goRight()
  {
    if (_queueDirection.size() < 3)
      _queueDirection.push(Direction::RIGHT);
    return true;
  }

  bool	Snake::_goForward()
  {
    return _play();
  }

  bool	Snake::_shoot()
  {
    return true;
  }

  bool	Snake::_illegal()
  {
    return true;
  }

  bool	Snake::_play()
  {
    return _move();
  }
}

extern "C"
{
  arcade::IGame	*createGame()
  {
    return new arcade::Snake;
  }

  void	Play(void)
  {
    static arcade::IGame *game = new arcade::Snake;

    uint16_t test;

    while (std::cin.read(reinterpret_cast<char*>(&test), 2))
      game->sendCmd(static_cast<arcade::CommandType>(test));
    delete game;
  }
}

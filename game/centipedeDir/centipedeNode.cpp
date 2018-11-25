#include "centipedeNode.hh"

namespace arcade
{
  centipedeNode::centipedeNode(uint16_t x, uint16_t y)
  {
    _pos.x = x;
    _pos.y = y;
    _direction.x = 1;
    _direction.y = 0;
    _typeSaved = TileType::EMPTY;
    _down = true;
  }

  bool		centipedeNode::_changeDirection(GetMap *map,
						uint32_t *score)
  {
    if (_down && _pos.y == map->height - 1)
      {
	_down = false;
	if (*score < 1)
	  *score = 0;
	else
	  *score -= 1;
      }
    else if (!_down && _pos.y < map->height - (20 * map->height / 100))
      _down = true;
    int down = (_down) ? 1 : -1;

    _direction.x = -_direction.x;
    map->tile[_pos.y * map->width + _pos.x] = _typeSaved;
    _pos.y += down;
    _typeSaved = map->tile[_pos.y * map->width + _pos.x];
    if (_typeSaved != TileType::OBSTACLE)
      _typeSaved = TileType::EMPTY;
    map->tile[_pos.y * map->width + _pos.x] = TileType::EVIL_DUDE;
    return true;
  }

  bool	centipedeNode::move(GetMap *map, uint32_t *score)
  {
    Position next;

    next.x = _pos.x + _direction.x;
    next.y = _pos.y + _direction.y;

    if ((_pos.x == 0 && _direction.x < 0) || next.x == map->width)
      return _changeDirection(map, score);
    else if ((_pos.y == 0 && _direction.y < 0) || next.y == map->height)
      return _changeDirection(map, score);
    else if (map->tile[next.y * map->width + next.x] == TileType::OBSTACLE)
      return _changeDirection(map, score);
    map->tile[_pos.y * map->width + _pos.x] = _typeSaved;
    _typeSaved = map->tile[next.y * map->width + next.x];
    if (_typeSaved != TileType::OBSTACLE)
      _typeSaved = TileType::EMPTY;
    if (map->tile[next.y * map->width + next.x] == TileType::MY_SHOOT)
      return (false);
    map->tile[next.y * map->width + next.x] = TileType::EVIL_DUDE;
    _pos.x = next.x;
    _pos.y = next.y;
    return (true);
  }

  centipedeNode::~centipedeNode()
  {
  }
}

#pragma once

#include "Protocol.hpp"

namespace arcade
{
  class centipedeNode {
  public:
    centipedeNode(uint16_t, uint16_t);
    ~centipedeNode();
    bool	move(GetMap *map, uint32_t *);
    Position	const &getPos() {return _pos;};
  private:
    typedef struct	Direction
    {
      char		x;
      char		y;
    }			Direction;

    bool		_changeDirection(GetMap *, uint32_t *);
    Position		_pos;
    Direction		_direction;
    TileType		_typeSaved;
    bool		_down;
  };
}

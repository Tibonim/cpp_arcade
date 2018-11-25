#include "Obstacle.hh"

namespace arcade
{
  Obstacle::Obstacle()
  {
    _shield = 5;
  }

  Obstacle::~Obstacle()
  {
  }

  bool Obstacle::destroy()
  {
    if (!(--_shield))
      return true;
    return false;
  }
}

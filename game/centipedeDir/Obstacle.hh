#pragma once

#include <cstdint>

namespace arcade
{
  class Obstacle {
  public:
    Obstacle();
    ~Obstacle();
    bool	destroy();
  private:
    uint16_t	_shield;
  };
}

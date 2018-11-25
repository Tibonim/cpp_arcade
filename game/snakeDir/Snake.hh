#pragma once

#include <queue>
#include <iostream>
#include "AGame.hh"

namespace	arcade {
  class		Snake : public AGame {
  public:
    Snake();
    ~Snake();
    void	clearSnake();
    void	restart();
  private:
    typedef struct	direction
    {
      int		x;
      int		y;
    }			direction;

    enum class Direction : char
    {
      RIGHT,
      LEFT,
      UP,
      DOWN
    };

    bool	_goUp();
    bool	_goDown();
    bool	_goLeft();
    bool	_goRight();
    bool	_goForward();
    bool	_shoot();
    bool	_illegal();
    bool	_play();

    /* Specific to this game*/
    void	_initGame();
    void	_initMap();
    void	_initPlayer();
    void	_putFood();
    bool	_snakeIsPresent(uint16_t, uint16_t, uint16_t);
    bool	_move();
    void	_changeDirection();
    bool	_willDie(int, int);

    direction	_direction;
    std::queue<Direction> _queueDirection;
    /* END Specific to this game*/
  };
}

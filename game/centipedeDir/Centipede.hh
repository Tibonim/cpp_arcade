#pragma once

#include <queue>
#include <iostream>
#include "AGame.hh"
#include "Obstacle.hh"
#include "centipedeNode.hh"

namespace	arcade {
  class		Centipede : public AGame {
  public:
    Centipede();
    ~Centipede();
    void restart();
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
    void	_initBlocks();
    void	_initSpawn();
    bool	_canMove(uint16_t, uint16_t);
    bool	_tryMove(Position *, uint16_t, uint16_t);
    void	_destroyObstacle(uint16_t);
    void	_destroyNode(uint16_t, uint16_t);
    void	_moveShoots();
    void	_moveNodes();
    bool	_alive(Position *);

    void	_generateNode();

    void	_shootObstacle(uint16_t, uint16_t, uint16_t);
    void	_shootNode(uint16_t, uint16_t, uint16_t);
    uint16_t	_limit;
    std::map<uint16_t, Obstacle *>	_blocks;
    std::vector<centipedeNode *>	_nodes;
    std::vector<Position *>		_shoots;

    typedef	void (Centipede::*func)(uint16_t, uint16_t, uint16_t);
    std::map<TileType, func>		_shootColide;
    uint16_t				_spawn;
    uint16_t				_cLen;
    uint16_t				_currLen;
    /* END Specific to this game*/
  };
}

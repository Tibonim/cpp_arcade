#pragma once

#include <random>
#include <vector>
#include <cstdint>
#include <map>
#include "scoreWriter.hh"
#include "IGame.hpp"
#include "Protocol.hpp"

namespace	arcade {
  class		AGame : public IGame {
  public:
    AGame(std::string const &, uint16_t w, uint16_t h);
    GetMap	const *getMap() const;
    WhereAmI	const *whereAmI() const;
    uint32_t	getScore() const;
    bool	update();
    ~AGame();
    bool	sendCmd(CommandType);
    virtual void restart() = 0;
    void	savePlayerName(std::string const& playerName) override;
    uint32_t	getBestScore() const override;
  protected:
    typedef	bool (AGame::*funcPtr)();
    bool	_getMap();
    bool	_whereAmI();
    virtual bool	_goUp() = 0;
    virtual bool	_goDown() = 0;
    virtual bool	_goLeft() = 0;
    virtual bool	_goRight() = 0;
    virtual bool	_goForward() = 0;
    virtual bool	_shoot() = 0;
    virtual bool	_illegal() = 0;
    virtual bool	_play() = 0;

    std::map<CommandType, funcPtr>	_tokens;
    std::vector<Position *>		_player;
    GetMap				*_map;
    uint32_t				_score;
    std::string				_playerName;
    std::string				_game;
  };
}

#pragma once

#include "Protocol.hpp"

namespace	arcade {
  class		IGame {
  public:
    virtual ~IGame() = default;
    virtual GetMap	const *getMap() const = 0;
    virtual bool	sendCmd(CommandType type) = 0;
    virtual uint32_t	getScore() const = 0;
    virtual WhereAmI	const *whereAmI() const = 0;
    virtual bool	update() = 0;
    virtual void	restart() = 0;
    virtual void	savePlayerName(std::string const& playerName) = 0;
    virtual uint32_t	getBestScore() const = 0;
  };
}

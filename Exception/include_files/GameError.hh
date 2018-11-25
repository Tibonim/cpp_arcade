#pragma once

#include "CustomException.hh"

namespace	arcade {
  class		GameError : public CustomException {
  public:
    GameError(std::string const& message, std::string const& comment);
  };
}

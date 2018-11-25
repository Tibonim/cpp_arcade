#pragma once

#include "CustomException.hh"

namespace	arcade {
  class		keyEventError : public CustomException {
  public:
    keyEventError(std::string const& message, std::string const& comment);
  };
}

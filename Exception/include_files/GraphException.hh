#pragma once

#include "CustomException.hh"

namespace	arcade {
  class		GraphError : public CustomException {
  public:
    GraphError(std::string const& message, std::string const& comment);
  };
}

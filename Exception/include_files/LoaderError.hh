#pragma once

#include "CustomException.hh"

namespace	arcade {
  class		LoaderError : public CustomException {
  public:
    LoaderError(std::string const& message, std::string const& comment);
  };
}

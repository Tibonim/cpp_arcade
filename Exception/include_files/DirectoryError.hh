#pragma once

#include "CustomException.hh"

namespace	arcade {
  class		DirectoryError : public CustomException {
  public:
    DirectoryError(std::string const& message, std::string const& comment);
  };
}

#include "GameError.hh"

namespace	arcade {
  GameError::GameError(std::string const& message, std::string const& comment)
    : CustomException{ message, comment } {}
}

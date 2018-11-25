#include "keyEventError.hh"

namespace	arcade {
  keyEventError::keyEventError(std::string const& message,
			       std::string const& comment)
    : CustomException{ message, comment } {}
}

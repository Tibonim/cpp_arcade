#include "LoaderError.hh"

namespace	arcade {
  LoaderError::LoaderError(std::string const& message, std::string const&
			   comment)
    : CustomException{ message, comment } {}
}

#include "DirectoryError.hh"

namespace	arcade {
  DirectoryError::DirectoryError(std::string const& message,
				 std::string const& comment)
    : CustomException{ message, comment } {}
}

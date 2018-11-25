#include "GraphException.hh"

namespace	arcade {
  GraphError::GraphError(std::string const& message,
			 std::string const& comment)
    : CustomException{ message, comment } {}
}

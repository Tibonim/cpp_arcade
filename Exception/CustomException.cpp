#include "CustomException.hh"

namespace	arcade {
  CustomException::CustomException(std::string const& message,
				   std::string const& comment)
    : _comment{ comment }, _message{ message } {}

  const char	*CustomException::what() const throw() {
    return _message.c_str();
  }

  const char	*CustomException::where() const throw() {
    return _comment.c_str();
  }
}

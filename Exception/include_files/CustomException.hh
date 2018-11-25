#pragma once

#include <string>
#include <stdexcept>

namespace	arcade {
  class		CustomException : public std::exception {
  public:
    CustomException(std::string const& message, std::string const& comment);
    virtual ~CustomException() throw() = default;
    const char	*where() const throw();
    const char	*what() const throw();
  private:
    std::string	const	_comment;
    std::string const	_message;
  };
}

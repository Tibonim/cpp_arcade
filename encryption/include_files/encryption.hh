#pragma once

#include <iterator>
#include <vector>
#include <fstream>
#include <string>

namespace	arcade {
  class		Encrypt {
  public:
    Encrypt(std::string const& fileIn,
	    std::string const& fileOut);
    void	setInput(std::string const& fileIn);
    void	setOutput(std::string const& fileOut);
    void	setKey(std::string const& key);
    void	run();
  private:
    void	encryptFile(std::vector<std::string> const& file,
			    std::string& valueToWrite) const;
    void	feedLine(std::string const& line, std::string& feed,
			 std::size_t idx) const;
    void	feedFile(std::vector<std::string>& file);
  private:
    std::string	_fileIn;
    std::string	_fileOut;
    std::string	_key;
  };
}

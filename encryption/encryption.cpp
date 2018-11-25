#include <iostream>
#include "encryption.hh"

namespace	arcade {
  Encrypt::Encrypt(std::string const& fileIn, std::string const& fileOut)
    : _fileIn{ fileIn }, _fileOut { fileOut }, _key{ "bibi" } {}

  void	Encrypt::setInput(std::string const& fileIn) {
    _fileIn = fileIn;
  }

  void	Encrypt::setOutput(std::string const& fileOut) {
    _fileOut = fileOut;
  }

  void	Encrypt::setKey(std::string const& key) {
    _key = key;
  }

  void		Encrypt::feedFile(std::vector<std::string>& file) {
    std::ifstream	ifs{ _fileIn };
    if (!ifs)
      return ;
    std::string		line{};
    while (std::getline(ifs, line))
      file.push_back(line);
  }

  void		Encrypt::run() {
    std::vector<std::string>	file{};
    feedFile(file);
    std::ofstream	ofs{ _fileOut };
    std::string		valueToWrite{};
    encryptFile(file, valueToWrite);
    ofs << valueToWrite;
  }

  void		Encrypt::encryptFile(std::vector<std::string> const& file,
				     std::string& valueToWrite) const {
    std::size_t	i{ 0 };
    for (auto const it : file) {
      std::string	line{};
      feedLine(it, line, i);
      valueToWrite += line + std::string("\n");
    }
  }

  void		Encrypt::feedLine(std::string const& line,
				  std::string& feed,
				  std::size_t idx) const {
    for (unsigned i = 0; i < line.size(); ++i) {
      int	value{ line[i] ^ _key[idx % _key.size()] };
      char	c = static_cast<char>(value);
      feed += std::string(1, c);
    }
  }
}

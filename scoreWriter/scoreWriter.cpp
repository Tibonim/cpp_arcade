#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "encryption.hh"
#include "scoreWriter.hh"

namespace	arcade {
  void		scoreWriter::eraseScore(std::string const& name,
					std::vector<std::string>& file) {
    for (auto it = file.begin(); it != file.end();) {
      if (it->find(name) != std::string::npos) {
	auto itTmp = it;
	++it;
	file.erase(itTmp);
	--it;
      }
      if (it != file.end())
	++it;
    }
  }

  void		scoreWriter::fillScore(std::vector<std::string>& file,
				       std::string const& fileName) {
    std::string		line;
    std::ifstream	ifs{ fileName };

    if (!ifs)
      return ;
    while (std::getline(ifs, line))
      file.push_back(line);
  }

  void		scoreWriter::writeValue(std::vector<std::string>& file,
					std::string& valueToWrite) {
    for (auto it : file)
      valueToWrite += it + std::string("\n");
  }

  void		scoreWriter::writeScore(std::string const& score,
					std::string const& game,
					std::string const& name) {
    std::string	fileName{ game + std::string(".score") };
    Encrypt	en{ std::string(".") + fileName,
	fileName };
    en.run();
    std::vector<std::string>	Vscore{};
    fillScore(Vscore, fileName);
    if (!Vscore.empty())
      eraseScore(name, Vscore);
    Vscore.push_back(name + std::string(":") + score);
    std::ofstream	ofs{ fileName };
    std::string	valueToWrite{};
    writeValue(Vscore, valueToWrite);
    ofs << valueToWrite;
    ofs.close();
    en.setInput(fileName);
    en.setOutput(std::string(".") + fileName);
    en.run();
  }
}

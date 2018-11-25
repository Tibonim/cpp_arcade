#pragma once

#include <vector>
#include <string>

namespace	arcade {
  class		scoreWriter {
  public:
    static void	writeScore(std::string const& score,
			   std::string const& game,
			   std::string const& name);
  private:
    static void	eraseScore(std::string const& name,
			   std::vector<std::string>& file);
    static void	fillScore(std::vector<std::string>& file,
			  std::string const& fileName);
    static void	writeValue(std::vector<std::string>& file,
			   std::string &valueToWrite);
  };
}

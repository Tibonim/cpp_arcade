#include "DirectoryError.hh"
#include "Dir.hh"

namespace	arcade {
  Dir::Dir(std::string const& path) {
    if (!(_directory = opendir(path.c_str())))
      throw DirectoryError{ "Cannot open: " + path, "Dir::Dir" };
  }

  Dir::~Dir() {
    closedir(_directory);
  }

  struct dirent	*Dir::readDir() const {
    return readdir(_directory);
  }
}

#pragma once

#include <string>
#include <dirent.h>

namespace	arcade {
  class		Dir {
  public:
    Dir(std::string const& path);
    ~Dir();
    struct dirent	*readDir() const;
  private:
    DIR			*_directory;
  };
}

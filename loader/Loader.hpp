#pragma once

#include <string>
#include <dlfcn.h>
#include "LoaderError.hh"

namespace	arcade {
  class		Controller;

  template<typename Interface>
  class		Loader {
  public:
    Interface		*create(std::string const& funcName) const;
    Interface		*create(std::string const& funcName,
				std::string const& imgPath) const;
    void		open(std::string const& library);
    void		close();
  private:
    void		*_dlopen(std::string const& filename, int flag) const;
    std::string		_dlerror() const;
    void		*_handle;
  };

  template<typename Interface>
  void	Loader<Interface>::open(std::string const& library) {
    if (!(_handle = _dlopen(library, RTLD_NOW)))
      throw LoaderError{ "Open library failed : " + _dlerror(),
	  "Loader::Loader" };
  }

  template<typename Interface>
  void	Loader<Interface>::close() {
    dlclose(_handle);
  }

  template<typename Interface>
  void		*Loader<Interface>::_dlopen(std::string const& fileName,
					    int flag) const {
    return dlopen(fileName.c_str(), flag);
  }

  template<typename Interface>
  std::string		Loader<Interface>::_dlerror() const {
    return std::string { dlerror() };
  }

  template<typename Interface>
  Interface	*Loader<Interface>::create(std::string const& f) const {
    Interface	*(*funcPtr) ();
    funcPtr = reinterpret_cast<Interface *(*)()>(dlsym(_handle, f.c_str()));
    if (!funcPtr)
      throw LoaderError { "Get symbol failed: " + _dlerror(),
	  "Loader::create" };
    return funcPtr();
  }

  template<typename Interface>
  Interface	*Loader<Interface>::create(std::string const& f,
					   std::string const& path) const {
    Interface	*(*funcPtr) (std::string const&);
    funcPtr = reinterpret_cast<Interface *
			       (*)(std::string const&)>
      (dlsym(_handle, f.c_str()));
    if (!funcPtr)
      throw LoaderError { "Get symbol failed: " + _dlerror(),
	  "Loader::create" };
    return funcPtr(path);
  }
}

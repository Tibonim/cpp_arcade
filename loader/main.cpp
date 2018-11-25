#include <iostream>
#include "CustomException.hh"
#include "Loader.hpp"

int		main() {
  try {
    arcade::Loader<int>	load{ "./libtest.so" };
    int		*test { load.create("dSomething") };
    std::cout << *test << std::endl;
  }
  catch (arcade::CustomException& bad) {
    std::cout << bad.what() << " in " << bad.where() << std::endl;
  }
}

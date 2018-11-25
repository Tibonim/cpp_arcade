#include <vector>
#include <string>
#include <iostream>
#include "Controller.hh"
#include "CustomException.hh"

int		main(int ac, char **av) {
  if (ac != 2) {
    std::cout << "USAGE: ./arcade graphic_library" << std::endl;
    return 0;
  }
  try {
    arcade::Controller	ctrl{ av[1] };
    ctrl.run();
  }
  catch (arcade::CustomException& bad) {
    std::cerr << bad.what() << " in " << bad.where() << std::endl;
  }
  return 0;
}

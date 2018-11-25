#include <dlfcn.h>
#include <iostream>
int	main(int ac, char **av)
{
  if (ac < 2)
    {
      std::cerr << "./a.out path/to/lib.so" << std::endl;
      return (-1);
    }
  void *t = dlopen(av[1], RTLD_NOW);
  void (*f)() = (void (*)())dlsym(t, "Play");
  f();
  dlclose(t);
  return (0);
}


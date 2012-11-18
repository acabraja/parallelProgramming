/* kompajliranje: icpc -pthread -c demo.cpp */
/* linkanje: ifort -standard-semantics -nofor-main -cxxlib demo.o forthr_c.o mymod.o */
/* Fortran kompajler, ali primijeti -nofor-main koji sprecava linkanje s Fortran main rutinom. */
/* Takodjer, -cxxlib nagovori Fortran kompajler da linka sa C++ standardnom bibliotekom. */

#include "forthr_c.h"

#include <cstdlib>
#include <iostream>

/* Prototip nase Fortran funkcije. */
/* Ovaj "C" je potreban da se ime ne bi `pokvarilo' u C++u. */
extern "C" void *dretva(void *arg);

/* Struktura tocka iz Fortran modula. */
struct tocka {
  double x, y, z;
};

int main(int argc, char *argv[])
{
  tocka t;
  pthread_t pth;
  void *ret;

  std::cout << "unesi x koordinatu: ";
  std::cin >> t.x;
  std::cout << "unesi y koordinatu: ";
  std::cin >> t.y;
  std::cout << "unesi z koordinatu: ";
  std::cin >> t.z;

  std::cout << "create_thread: " << create_thread(dretva, &t, &pth) << std::endl;
  std::cout << "join_thread: " << join_thread(pth, &ret) << std::endl;

  std::cout << "korijen iz sume kvadrata koordinata = " << *(double*)ret << std::endl;
  free(ret);
}

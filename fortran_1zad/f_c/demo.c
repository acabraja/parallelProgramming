/* kompajliranje: icc -pthread -c demo.c */
/* linkanje: ifort -standard-semantics -nofor-main demo.o forthr_c.o mymod.o */
/* Fortran kompajler, ali primijeti -nofor-main koji sprecava linkanje s Fortran main rutinom. */

#include "forthr_c.h"

#include <stdlib.h>
#include <stdio.h>

/* Prototip nase Fortran funkcije. */
/* Ovaj "C" je potreban da se ime ne bi `pokvarilo' u C++u. */
extern
#ifdef __cplusplus
"C"
#endif /* __cplusplus */
void *dretva(void *arg);

/* Struktura tocka iz Fortran modula. */
typedef struct {
  double x, y, z;
} tocka;

int main(int argc, char *argv[])
{
  tocka t;
  pthread_t pth;
  void *ret;

  printf("unesi x koordinatu: ");
  scanf("%le", &t.x);
  printf("unesi y koordinatu: ");
  scanf("%le", &t.y);
  printf("unesi z koordinatu: ");
  scanf("%le", &t.z);

  printf("create_thread: %d\n", create_thread(dretva, &t, &pth));
  printf("join_thread: %d\n", join_thread(pth, &ret));

  printf("korijen iz sume kvadrata koordinata = %#.16e\n", *(double*)ret);
  free(ret);

  return EXIT_SUCCESS;
}

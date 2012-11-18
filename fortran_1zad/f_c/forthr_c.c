/*
  za koristenje: icc -pthread -c forthr_c.c
  za testiranje: icc -pthread -DFORTHR_C_TEST forthr_c.c
*/

#include "forthr_c.h"

#ifdef FORTHR_C_TEST
#include <stdio.h>
#include <stdlib.h>
#endif /* FORTHR_C_TEST */

int create_thread(const pth_fn_t fn, void *const arg, pthread_t *const pth)
{
  return pthread_create(pth, (const pthread_attr_t*)NULL, fn, arg);
}

int join_thread(const pthread_t pth, void **const ret)
{
  return pthread_join(pth, ret);
}

#ifdef FORTHR_C_TEST
int main(int argc, char *argv[])
{
  /* Fortran interface ce raditi na 64-bitnim sustavima samo ako vrijedi sizeof(pthread_t) == 8 */
  (void)printf("sizeof(pthread_t): %u\n", (unsigned)sizeof(pthread_t));
  return EXIT_SUCCESS;
}
#endif /* FORTHR_C_TEST */

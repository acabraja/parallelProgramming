/* gcc -pthread proba1.c -o proba1 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Header file za POSIX threadove. */
#include <pthread.h>


/* Tip pokazivaca na startnu rutinu za dretvu. */
typedef void *(*StartRoutine)(void*);


void *dretva(void *arg) {
  (void)scanf("%%");
  return arg;
}


int main(void) {
  pthread_t thread;
  if (pthread_create(&thread, (const pthread_attr_t*)NULL, dretva, NULL)) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }
  /* Program bez donje linije zavrsi odmah.
     Poanta: pricekati da svi pokrenuti threadovi zavrse!
     (void)pthread_join(thread, NULL);
  
	(void)pthread_join(thread,&retval);
	printf("%p\n", retval);
	return EXIT_SUCCESS;
	
	
*/
  return EXIT_SUCCESS;
}

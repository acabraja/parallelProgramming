#include<stdio.h>
#include<pthread.h>

#define create_pth create_pth_
#define join_pth join_pth_
#define data data_

extern struct{
  int i;
}data_;

void create_pth(void *(*thread_func)(void *), pthread_t *theThread)
{
  pthread_create(theThread,NULL,thread_func,NULL);
}

void join_pth(pthread_t *theThread)
{
  int value = 0;
  pthread_join(*theThread,(void **)&value);
}

void test_(void)
{
  printf("%d",data.i);
}

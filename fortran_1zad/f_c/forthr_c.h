#ifndef FORTHR_C_H
#define FORTHR_C_H

#include <pthread.h>

typedef void *(*pth_fn_t)(void*);

extern
#ifdef __cplusplus
"C"
#endif /* __cplusplus */
int create_thread(const pth_fn_t fn, void *const arg, pthread_t *const pth);

extern
#ifdef __cplusplus
"C"
#endif /* __cplusplus */
int join_thread(const pthread_t pth, void **const ret);

#endif /* !FORTHR_C_H */

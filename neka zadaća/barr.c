
#define pthread_barrier_t barrier_t
#define pthread_barrier_attr_t barrier_attr_t
#define pthread_barrier_init(b,a,n) barrier_init(b,n)
#define pthread_barrier_destroy(b) barrier_destroy(b)
#define pthread_barrier_wait(b) barrier_wait(b)


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct {
    int needed;
    int called;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} barrier_t;

int barrier_init(barrier_t *barrier,int needed);
int barrier_destroy(barrier_t *barrier);
int barrier_wait(barrier_t *barrier);

int barrier_init(barrier_t *barrier,int needed)
{
    barrier->needed = needed;
    barrier->called = 0;
    pthread_mutex_init(&barrier->mutex,NULL);
    pthread_cond_init(&barrier->cond,NULL);
    return 0;
}

int barrier_destroy(barrier_t *barrier)
{
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
    return 0;
}

int barrier_wait(barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    barrier->called++;
    if (barrier->called == barrier->needed) {
        barrier->called = 0;
        pthread_cond_broadcast(&barrier->cond);
    } else {
        pthread_cond_wait(&barrier->cond,&barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}

struct worker_data {
	int *data;
	int start;
	int num;
	int tid;
	int nthr;
	pthread_t *threads;
	pthread_barrier_t *before_reduction;
};

int *worker(struct worker_data *d);

int main(int argc, char *argv[])
{
	int ndata, nthr;
	pthread_t *pt;
	int *data;
	struct worker_data *wd;
	int i;
	int next;
	int *result;
	pthread_barrier_t red_barr;

	if (argc != 3 || sscanf(argv[1], "%d", &ndata) != 1 || ndata < 1 ||
			sscanf(argv[2], "%d", &nthr) != 1 || nthr < 1) {
		fprintf(stderr, "usage: %s N P\n  N, P: natural numbers\n"
				"  Calculates sum of squares of first N natural"
				" numbers, using P threads.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pt = (pthread_t *) malloc(nthr * sizeof(pthread_t));
	wd = (struct worker_data *) malloc(nthr * sizeof(struct worker_data));
	data = (int *) malloc(ndata * sizeof(int));
	if (pt == NULL || wd == NULL || data == NULL) {
		fprintf(stderr, "main(): malloc failed\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < ndata; i++)
		data[i] = i+1;

	pthread_barrier_init(&red_barr, NULL, nthr);

	next = 0;
	for (i = 0; i < nthr; i++) {
		wd[i].tid = i;
		wd[i].nthr = nthr;
		wd[i].data = data;
		wd[i].start = next;
		wd[i].num = (ndata - next) / (nthr - i);
		next += wd[i].num;
		wd[i].threads = pt;
		wd[i].before_reduction = &red_barr;
		pthread_create(&pt[i], NULL, (void * (*)(void *)) worker, &wd[i]);
	}

	pthread_join(pt[0], (void **) &result);
	printf("%d\n", *result);

	free(result);
	free(pt);
	free(wd);
	free(data);
	pthread_barrier_destroy(&red_barr);

	return EXIT_SUCCESS;
}

int *worker(struct worker_data *d) {
	int *result;
	int *tmp;
	int i;
	int step;
	int dist;

	printf("Thread %d: working on %d elements: %d to %d.\n",
			d->tid, d->num, d->start, d->start + d->num - 1);
	result = (int *) malloc(sizeof(int));
	if (result == NULL) {
		fprintf(stderr, "worker(): malloc failed\n");
		exit(EXIT_FAILURE);
	}

	*result = 0;
	for (i = 0; i < d->num; i++)
		*result += d->data[d->start + i] * d->data[d->start + i];

	pthread_barrier_wait(d->before_reduction);

	step = 0;
	dist = 1;
	while (1) {
		if ((d->tid >> step) % 2 == 0 && d->tid + dist < d->nthr) {
			pthread_join(d->threads[d->tid + dist], (void **) &tmp);
			*result += *tmp;
			free(tmp);
			printf("Step %d: Thread %d: joining with thread %d.\n",
					step, d->tid, d->tid + dist);
		} else
			pthread_exit((void *) result);

		step++;
		dist *= 2;
	}
}

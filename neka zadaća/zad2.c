
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

struct thread_data{
    int length, where_to_start,id;
    int num, thread_id;s
    double float *real, *imag;
    double float **others;
    int row;
    pthread_barrier_t *before_reduction;
    pthread_t *threads;
};

int min_search( struct thread_data *arg){
    
    int *result;
    result = (double float *)malloc(4*sizeof(double float));
    if (result == NULL) {
        printf("\n Greska pri alociranju memorije za rezultat \n");
        exit(0);
    }
    
    result[0] = 0;
    int i;
    struct thread_data *my_arg; 
    my_arg = (struct thread_data *) arg;
    double float x,y;
    for (i = 0; i < my_arg->num; i++){
        x = my_arg->real[my_arg->where_to_start + i]; 
        y = my_arg->imag[my_arg->where_to_start + i];
        if ( sqrt(x*x + y*y) > result[0] ){
            result[0] = sqrt(x*x + y*y);
            result[1] = x;
            result[2] = y;
            result[3] = my_arg->where_to_start + i;
        }
        else if (sqrt(x*x + y*y) = result[0]){
        
            my_arg->others[row][0] = sqrt(x*x + y*y;
            my_arg->others[row][1] = x;
            my_arg->others[row][2] = y;
            my_arg->others[row][3] = my_arg->where_to_start+i; 
            
        }
        
        pthread_barrier_wait(my_arg->before_reduction);
        
        int step = 0;
        int dist = 0;
        while(1) {
            
            if ((my_arg->thread_id >> step) % 2 == 0 && my_arg->thread_id + dist < thread_id->length) {
                pthread_join(my_arg->threads[my_arg->thread_id + dist], (void **) &tmp);
                *result += *tmp;
                free(tmp);
            }
            else{
                
                pthread_exit((void *) result);
                
            }
            step++;
            dist *= 2;
        
        }

int main(int argc, char **argv) {
	
	
	// ************ deklaracija varijabli ***************
	
	int counter1 = 0;
	int counter2 = 0;
	char c;
	double float *R = NULL;
	double float *I = NULL;
	int flag = 0;
	int counter3 = 0;
	int n,p,t;
	const char EOL = '\n';
	char *number = NULL;

	if (argc != 4) {
		
		printf("Greska! Nisu uneseni svi potrebni argumenti.");
		exit(0);
		
	}
	
	n = atoi(argv[1]);
	p = atoi(argv[2]);
	

	// *************** otvaranje datoteka ****************

	FILE *pFile;
	pFile = fopen(argv[3], "r");
	if ( pFile == NULL ) {
		
		printf("Nije moguce otvoriti datoteku\n");
		exit(0);
		
	}
	
	// *********** ispis vektora ************************

	printf("\n ********** Vektor ********** \n");
	while(1){
		if (c == EOF) {
			c = ' ';
			break;
		}
		c = getc(pFile);
		printf("%c", c);
	}

	fclose(pFile);
	
	// ************** ucitavanje vektora ***************
 
	pFile = fopen(argv[3], "r");

	while(1) {
		
		c = getc(pFile);
		if ( c == EOF ) {
			 break;
		}
		if ( c == ','){
			number[counter3] = '\0';
			float num_f = atof(number);
			R = (float*)realloc(R, ++counter1 * sizeof(float));	
			R[counter1 - 1] = num_f;
			counter3 = 0;	
			
		}	
		if (c == ')'){
		    number[counter3] = '\0';
			float num_f = atof(number);
			I = (float*)realloc(I, ++counter2 * sizeof(float));	
			I[counter2 - 1] = num_f;
			counter3 = 0;
			number = NULL;
		}		
		if (c != '(' && c != ',' && c != ')' && c != ' ') {
			number = (char*)realloc(number, ++counter3 * sizeof(char));
			number[counter3-1] = c;
		}
		
	}
	pthread_t threads[p];
	struct thread_data thread_d[p];
	int next = 0;
	pthread_barrier_t reduction_barrier;
	pthread_barrier_init(&reduction_barrier, NULL, nthr);
	double float **others;
	
	for (t = 0; t < p; t++) {
	            
	            thread_d[t].thread_id = t;
	            thread_d[t].real = R;
	            thread_d[t].imag = I;
	            thread_d[t].length = n;
	            thread_d[t].where_to_start = next;
	            thread_d[t].num = (n - next) / (p - t);
	            thread_d[t].threads = threads;
	            thread_d[t].others = others;
		        next += thread_d[t].num;
		        thread_d[t].before_reduction = &reduction_barrier;
	            
	            pthread_create(&threads[t], (const pthread_attr_t*)NULL, min_search, &thread_d[t]);
	}  
	
	pthread_join(pt[0], (void **) &result);
	//free-anje memorije
	pthread_barrier_destroy(&red_barr);
	pthread_exit(NULL);

}

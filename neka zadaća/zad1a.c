#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "f2c.h"
#include "fblaswr.h"
#include "clapack.h"



struct thread_data {
    
    int dim, dim_max, shift;
    float *R1, *I1, *R2, *I2;
};


double  *rez_real = NULL;
double  *rez_img = NULL;

float* array_for_thread(float *array, int n, int shift, int p) {
	
	int i,j,br;
	float *array_t;
	int m;
	m = n/p;
	array_t = (float*)malloc(m*m*sizeof(float));
	i = -1;
	br = 0;
	int counter;
	counter = 1;
	
	while(1){
		for(j = 0; j < m; j++){
		    i = i+1;
		    
		    array_t[br] = array[shift + i];
		    
		    br = br+1;
		}
		i = n-m+i;
		if (br == m*m) break;
	}
    
	return array_t;
		 
	
}

void *multiply( void *arg) {

   int i = -1,j;
   struct thread_data *my_arg; 
   my_arg = (struct thread_data *) arg;
   
   int n,shift;
   n = my_arg->dim_max;
   shift = my_arg->shift;

   integer m = my_arg->dim;
   char transaN = 'N';
   doublereal alfa = 1.0;
   doublereal beta = 0.0;
   doublereal *C1,*C2,*C3,*C4;
   
   
   C1 = (doublereal*)malloc(m*m*sizeof(doublereal));
   C2 = (doublereal*)malloc(m*m*sizeof(doublereal));
   C3 = (doublereal*)malloc(m*m*sizeof(doublereal));
   C4 = (doublereal*)malloc(m*m*sizeof(doublereal));
   doublereal *r1 = (doublereal*)my_arg->R1;
   doublereal *r2 = (doublereal*)my_arg->R2;
   doublereal *i1 = (doublereal*)my_arg->I1;
   doublereal *i2 = (doublereal*)my_arg->I2;  
   
    int k,kk;
    for (k = 0;k<m; k++){
        printf("\n");
        for (kk= 0; kk<m; kk++){
            printf("%d  %f + i*%f ",my_arg->shift, my_arg->R1[k*m+kk], my_arg->I1[k*m+kk]);
        }
    }
    printf("\n imaginarne: ");
   
    for (k = 0;k<m; k++){
        printf("\n");
        for (kk= 0; kk<m; kk++){
            printf("%d  %f+ i*%f ",my_arg->shift, my_arg->R2[k*m+kk], my_arg->I2[k*m+kk]);
        }
    }
    printf("\n");
    
   dgemm_(&transaN, &transaN, &m,&m,&m,&alfa, r1,&m,r2, &m,&beta, C1, &m); 
  
   dgemm_(&transaN, &transaN, &m,&m,&m,&alfa, i1,&m,i2, &m,&beta, C2, &m);
   
   dgemm_(&transaN, &transaN, &m,&m,&m,&alfa, i1,&m,r1, &m,&beta, C3, &m);
   	
   dgemm_(&transaN, &transaN, &m,&m,&m,&alfa, r1,&m,i2, &m,&beta, C4, &m);
   	
        printf("\n rezultat: ");
   
    for (k = 0;k<m; k++){
        printf("\n");
        for (kk= 0; kk<m; kk++){
            printf("%d  %f+ i*%f ",my_arg->shift, C1[k*m+kk], C2[k*m+kk]);
        }
    }
    printf("\n");
   int counter = 0;
    while(1){
		    for(j = 0; j < m; j++){
		        
		        
		        i = i+1;
		        rez_real[i+shift] =C1[counter] - C2[counter];
		        rez_img[i+shift] = C3[counter] + C4[counter];

		        counter = counter + 1;
		    }
		    i = n-m+i;
		    if (counter == m*m) break;
    }   
    pthread_exit(NULL);
}


int main(int argc, char **argv) {
	
	if (remove("Rezultat.txt") == -1) {
		perror("\n File Rezultat.txt mozda sadrzi prijasnje rezultate \n");
		}		
	// ************ deklaracija varijabli ***************
	
	int counter1 = 0;
	int counter2 = 0;
	char c;
	float *R1 = NULL;
	float *I1 = NULL;
	float *R2 = NULL;
	float *I2 = NULL;
	int flag = 0;
	int counter3 = 0;
	int n,p;
	const char EOL = '\n';
	char *number = NULL;
	
	if (argc != 5) {
		
		printf("Greska! Nisu uneseni svi potrebni argumenti.");
		exit(0);
		
	}
	
	n = atoi(argv[1]);
	p = atoi(argv[2]);
	
	if (n%p != 0 ) {
		printf(" p ne dijeli n. Kraj izvodjenja programa!");
		exit(0);
	}
    
    rez_real = (doublereal*) malloc (n*n*sizeof(doublereal));
    rez_img = (doublereal*) malloc (n*n*sizeof(doublereal));
    
        

	// *************** otvaranje datoteka ****************

	FILE *pFile;
	FILE *pFile2;
	pFile2 = fopen(argv[4], "r"); 
	pFile = fopen(argv[3], "r");
	if ( pFile == NULL ) {
		
		printf("Nije moguce otvoriti datoteku\n");
		exit(-1);
		
	}
	
	// *********** ispis matrica ************************

	printf("\nPrva matrica\n");
	while(1){
		if (c == EOF) {
			c = ' ';
			break;
		}
		c = getc(pFile);
		printf("%c", c);
	}
	printf("\nDruga matrica\n");
	while(1){
		if (c == EOF) {
			break;
		}
		c = getc(pFile2);
		printf("%c", c);
	}
	
	fclose(pFile2);
	fclose(pFile);
	
	// ************** ucitavanje matrica  ***************
	
	pFile2 = fopen(argv[4], "r"); 
	pFile = fopen(argv[3], "r");

	while(1) {
		
		c = getc(pFile);
		if ( c == EOF ) {
			 break;
		}
		if ( c == ','){
			number[counter3] = '\0';
			float num_f = atof(number);
			R1 = (float*)realloc(R1, ++counter1 * sizeof(float));	
			R1[counter1 - 1] = num_f;
			counter3 = 0;	
			
		}	
		if (c == ')'){
			number[counter3] = '\0';
			float num_f = atof(number);
			I1 = (float*)realloc(I1, ++counter2 * sizeof(float));	
			I1[counter2 - 1] = num_f;
			counter3 = 0;
			number = NULL;
		}		
		if (c != '(' && c != ',' && c != ')' && c != ' ') {
			number = (char*)realloc(number, ++counter3 * sizeof(char));
			number[counter3-1] = c;
		}
		
	}
	
	counter1 = 0;
	counter2 = 0;
	counter3 = 0;
	
	while(1) {
		
		c = getc(pFile2);
		if ( c == EOF ) {
			 break;
		}
		if ( c == ','){
			number[counter3] = '\0';
			float num_f = atof(number);
			R2 = (float*)realloc(R2, ++counter1 * sizeof(float));	
			R2[counter1 - 1] = num_f;
			counter3 = 0;	
			
		}	
		if (c == ')'){
			number[counter3] = '\0';
			float num_f = atof(number);
			I2 = (float*)realloc(I2, ++counter2 * sizeof(float));	
			I2[counter2 - 1] = num_f;
			counter3 = 0;
			number = NULL;
		}		
		if (c != '(' && c != ',' && c != ')' && c != ' ') {
			number = (char*)realloc(number, ++counter3 * sizeof(char));
			number[counter3-1] = c;
		}
		
	}
	
	fclose(pFile);
	fclose(pFile2);


	int t,i,j;
	pthread_t threads[p*p];	
	float *array_tr1, *array_ti1, *array_tr2, *array_ti2;
	int m = n/p;
	struct thread_data thread_d[p*p];
	
    int shift = -m;    	  
	for (t = 0; t < p*p; t++) {
	
	    if ((t*m)%n == 0 && t != 0) {
	        
	        shift = shift + n+m;
	        array_tr1 = array_for_thread(R1,n,shift, p);
	        array_ti1 = array_for_thread(I1,n,shift, p);
	        array_tr2 = array_for_thread(R2,n,shift, p);
	        array_ti2 = array_for_thread(I2,n,shift, p);
	        
	    }
	    else {
	        
	        shift = shift + m;
	        array_tr1 = array_for_thread(R1,n,shift, p);
	        array_ti1 = array_for_thread(I1,n,shift, p);
	        array_tr2 = array_for_thread(R2,n,shift, p);
	        array_ti2 = array_for_thread(I2,n,shift, p);
      
        }
	  
	 
	    thread_d[t].R1 = array_tr1;
	    thread_d[t].I1 = array_ti1;
	    thread_d[t].R2 = array_tr2;
	    thread_d[t].I2 = array_ti2;
	    thread_d[t].dim = m;
	    thread_d[t].dim_max = n;
	    thread_d[t].shift = shift;
	    
	  
	    int thread = pthread_create(&threads[t], (const pthread_attr_t*)NULL, multiply, &thread_d[t]);  
	
		if (thread) {
			
			printf("Greska br. %d \n pri kreiranju dretve", thread);
			exit(0);
		
		}
	    
	}
	for (t = 0; t < p*p; t++) {
		
		int join = pthread_join(threads[t], NULL);
		if (join) {
			
			printf("Greska br. %d \n pri izvodjenju pthread_join", join);
			exit(0);
			
		}
		else if (t == p*p-1) {
		
	        FILE *rez;
	        rez = fopen("Rezultat.txt", "w"); 
	        for (i = 0; i<n;i++){
			    fprintf(rez, "\n");
			    for(j = 0;j<n; j++){
					fprintf(rez, "( %f ,", rez_real[j+n*i]);
			        fprintf(rez, "%f ) ", rez_img[j+i*n]);
			    }
			}
        
			fclose(rez);
			
		}
		
	}	
	

FILE *rez;
rez = fopen("Rezultat.txt", "r");
if ( rez == NULL ) {
		
		printf("Nije moguce otvoriti datoteku\n");
		exit(-1);
		
}
printf("\n ******** Rezultat ******** \n");
c = ' ';
while(1){
	if (c == EOF) {
		c = ' ';
		break;
	}
	c = getc(rez);
	if (c == EOL) 
	    printf("\n");
	else
	    printf("%c", c);
}

fclose(rez);	

free(array_tr1);
free(array_tr2);
free(array_ti1);
free(array_ti2);
free(R1);
free(R2);
free(I1);
free(I2);

pthread_exit(NULL);

}

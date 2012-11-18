#include<iostream>
#include<string>
#include<pthread.h>
#include<cblas.h>
#include<stdio.h>
#include<cstdlib>
#include<fstream>
#include<iomanip>

using namespace std;

// globalne varijable

static pthread_barrier_t *barijere,barijera_za_poravnanje;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER,lock1 = PTHREAD_MUTEX_INITIALIZER;
int koliko_je_gotovo=0; // da se ne koristi nepotrebna memorija nego da sve matrice znaju koji dio vektora B je rješenje a koji nije

struct Data{
  int koja_sam,start,end,velicina,N;
  double *A,*B;
};

void *thread_main(Data *const d)
{
  
  int kg = 0;
  pthread_barrier_wait(&barijera_za_poravnanje);  
  
  for(int i=1; i<=d->koja_sam; i++)
  {
    if( i == d->koja_sam )
    {
      pthread_mutex_lock(&lock1);
      cout<<"Racuna "<<d->koja_sam<<". Dretva"<<endl;
      pthread_mutex_unlock(&lock1);
      cblas_dtrsv(CblasRowMajor,CblasUpper,CblasNoTrans,CblasNonUnit,d->velicina,&d->A[(d->end+1)*d->N+d->N-kg-d->velicina],d->N,&d->B[d->end+1],1); 
      pthread_mutex_lock(&lock);
      koliko_je_gotovo+=d->velicina;
      pthread_mutex_unlock(&lock);
      pthread_barrier_wait(&barijere[i-1]);
    }
    else
    {
      pthread_barrier_wait(&barijere[i-1]);
      pthread_mutex_lock(&lock);
      int pomak=koliko_je_gotovo-kg;
      kg=koliko_je_gotovo;
      pthread_mutex_unlock(&lock);
      //pthread_mutex_lock(&lock1);
      //cout<<"Ceka "<<d->koja_sam<<". Dretva"<<endl;
      //pthread_mutex_unlock(&lock1);
      cblas_dgemv(CblasRowMajor,CblasNoTrans,d->velicina,pomak,-1,&d->A[(d->end+1)*d->N + d->N-kg],d->N,&d->B[d->N-kg],1,1,&d->B[d->end+1],1);
    }
  }
  pthread_mutex_lock(&lock1);
  cout<<"Zavrsila "<<d->koja_sam<<". Dretva"<<endl;
  pthread_mutex_unlock(&lock1);
}


int main(int argc,char *argv[])
{
  if(argc != 6){
   cout<<"Greska! Netocan broj parametara"<<endl;
   return EXIT_FAILURE;
  }
  
  int N,P;
  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&P);
  double *A,*B;
  
  if(N < P || P > 256) 
  {
    cout<<"Ima više dretvi nego sto smije biti "<<endl;
    return EXIT_FAILURE;
  }
  
  ifstream matrix1(argv[3]),vector1(argv[4]);
  ofstream rezultat(argv[5]);
  
  //rezultat << fixed << showpoint;
  
  A = new double[N*N];
  B = new double[N];
  
  if(matrix1.is_open() && vector1.is_open() && rezultat.is_open())
  {
    for(int i = 0; i < N; i++)
		  for(int j = 0;j < N; j++)
		  {
			  double z;
			  matrix1>>z;
			  A[(i*N)+j] = z;	
		  }
    matrix1.close();
    
     for(int i = 0; i < N; i++)
		 {
			  double z;
			  vector1>>z;
			  B[i] = z;	
		  }
    vector1.close();
  }
  else
  {
    cout<<"Ne mogu otvoriti txt datoteke"<<endl;
    return EXIT_FAILURE;
  }
  
  pthread_barrier_init(&barijera_za_poravnanje,NULL,P);


  Data *dat;
  pthread_t *pth;
  
  
  if (!(pth = (pthread_t*)malloc(P * sizeof(pthread_t)))) 
  {
      cout<<"Greska pri stvaranju dretvi!"<<endl;
      return EXIT_FAILURE;
  } 
 if (!(barijere = (pthread_barrier_t*)malloc(P * sizeof(pthread_barrier_t)))) 
  {
      cout<<"Greska pri stvaranju dretvi!"<<endl;
      return EXIT_FAILURE;
  } 
  for(int j=0;j<P;j++)
    pthread_barrier_init(barijere+j,NULL,P-j);

 int s_start = N-1;
 int mnoz = N/P;
 int rem = N%P;
 for (int i = 0; i < P; ++i) 
 {
    if (!(dat = (Data*)malloc(sizeof(Data)))) 
    {
      cout<<"Greska pri stvaranju struktura!"<<endl;
      return EXIT_FAILURE;
    }
    dat->koja_sam=i+1;    
    dat->start = s_start;
    dat->A = A;
    dat->B = B;
    dat->N=N;
    if(rem)
    {
      s_start = dat->end = s_start-mnoz - 1;
      rem--;
    }
    else
      s_start = dat->end = s_start - mnoz;
    
    
    dat->velicina = dat->start-dat->end;
    
    cout<<"Dretva["<<i<<"]"<<" = "<<dat->start<<","<<dat->end<<" i moja velicina je "<<dat->velicina<<endl;
    
    if (pthread_create(pth + i, NULL, (void*(*)(void*))thread_main, dat)) 
    {
      cout<<"Greska pri pokretanju dretve!"<<endl;
      return EXIT_FAILURE;
    }
 }
  
   for (int i = 0; i < P; ++i) 
   {
    if (pthread_join(pth[i], NULL)) 
    {
      cout<<"Join error!"<<endl;
      return EXIT_FAILURE;
    }
   }
  
  
  for(int i=0;i<N;i++)
    rezultat<<setprecision(16)<<B[i]<<endl;
  
 return 0;
}

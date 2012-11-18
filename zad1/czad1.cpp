#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cblas.h>
#include<pthread.h>
#include<complex>
#include<iomanip>

using namespace std;

struct Data{
  int start,end,lenght;
  complex<double> *A,*B,**C;
};

void *thread_main(Data *const d) 
{
  complex<double> *POM;
  POM = new complex<double>[d->lenght*d->lenght];
  
  for(int i=d->start;i<d->end;i++)
  {
    for( int l = 0; l < d->lenght*d->lenght; l++)
      POM[l]=d->B[l];
    
    cblas_zscal(d->lenght*d->lenght,&d->A[i],POM,1);
    
    for(int j=0; j<d->lenght;j++)
      for(int k=0;k<d->lenght;k++)
        d->C[(i/d->lenght)*d->lenght+j][(i%d->lenght)*d->lenght+k]=POM[j*d->lenght+k];
  }
  delete[] POM;
}

int main(int argc, char *argv[])
{
  complex<double> *A,*B,**C;
  int N,P,i,rem;
  pthread_t *pth;
  Data *dat;

  if(argc != 6)
  {
    cout<<"Unešeni broj parametara nije valjan!"<<endl;
    return EXIT_FAILURE;
  }
  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&P);
  
  if(N*N < P) 
  {
    cout<<"Ima više dretvi nego elemenata "<<endl;
    return EXIT_FAILURE;
  }
  
  rem = (N*N) % P;
  A = new complex<double>[N*N];
  B = new complex<double>[N*N];
  C = new complex<double>*[N*N];
  
  for(int k = 0; k < N*N; k++) 
    C[k] = new complex<double>[N*N];
  
  ifstream matrix1(argv[3]),matrix2(argv[4]);
  ofstream rezultat(argv[5]);
  
  if(matrix1.is_open() && matrix2.is_open() && rezultat.is_open())
  {
    for(int i = 0; i < N; i++)
		  for(int j = 0;j < N; j++)
		  {
			  complex<double> z;
			  matrix1 >> z;
			  A[(i*N)+j] = z;	
		  }
    matrix1.close();
    
     for(int i = 0; i < N; i++)
		  for(int j = 0;j < N; j++)
		  {
			  complex<double> z;
			  matrix2 >> z;
			  B[(i*N)+j] = z;	
		  }
    matrix2.close();
  }
  else
  {
    cout<<"Ne mogu otvoriti txt datoteke"<<endl;
    return EXIT_FAILURE;
  }
 
 if (!(pth = (pthread_t*)malloc(P * sizeof(pthread_t)))) 
 {
      cout<<"Greska pri stvaranju dretvi!"<<endl;
      return EXIT_FAILURE;
 } 
 
 int s_start = 0;
 int mnoz = N*N/P;
 for (i = 0; i < P; ++i) 
 {
    if (!(dat = (Data*)malloc(sizeof(Data)))) 
    {
      cout<<"Greska pri stvaranju struktura!"<<endl;
      return EXIT_FAILURE;
    }
    
    dat->start = s_start;
    dat->A = A;
    dat->B = B;
    dat->C = C;
    dat->lenght = N;
    
    if(rem)
    {
      s_start = dat->end = s_start+mnoz + 1;
      rem--;
    }
    else
      s_start = dat->end = s_start + mnoz;
    
    cout<<"Dretva["<<i<<"]"<<" = "<<dat->start<<","<<dat->end<<endl;
    
    if (pthread_create(pth + i, NULL, (void*(*)(void*))thread_main, dat)) 
    {
      cout<<"Greska pri pokretanju dretve!"<<endl;
      return EXIT_FAILURE;
    }
 }
 
  for (i = 0; i < P; ++i) 
  {
    if (pthread_join(pth[i], NULL)) 
    {
      cout<<"Join error!"<<endl;
      return EXIT_FAILURE;
    }
  }
  
  for(i=0;i<N*N;i++)
  {
    for(int j=0;j<N*N;j++)
      rezultat<<setprecision(16)<<C[i][j]<<" ";
    
    rezultat<<endl;
   }    
}

/*
#include <pthread.h>

// Create other necessary functions here


// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    
}
*/

#include <pthread.h>

// Create other necessary functions here
struct arguments{
	int id,N;
	int *mA,*mB,*mO;
};

#define max_thread 16
void* multiT(void *args)
{	arguments *t=(struct arguments *) args;
	int *matA=t->mA;
	int *matB=t->mB;
	int *output=t->mO;
	int N=t->N;
	int start=(t->id)*2*N/max_thread; 
	int end=(t->id +1 )*2*N/max_thread;
	//dealing with only one thread case
   if(max_thread==1)
   {
   	 for(int i = 0; i < N; ++i) {
        int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; ++j) {
            int rowA = j;
            int colA = i - j;
            int rowB = i - j;
            int colB = N - j - 1;
            temp += matA[rowA * N + colA] * matB[rowB * N + colB];
        }
        output[i] = temp;
    }
    
    // Iterate over second half of output elements
    for(int i = N; i < 2 * N - 1; ++i) {
        int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * N - (i + 1); ++j) {
            int rowA = i + 1 + j - N;
            int colA = N - j - 1;
            int rowB = N - j - 1;
            int colB = 2 * N - j - 2 - i;
            temp += matA[rowA * N + colA] * matB[rowB * N + colB];
        }
        output[i] = temp;
    }
   }
   
   else if(start<N)
   {
     for(int i = start; i < end; ++i) {
        int temp = 0;
        for(int j = 0; j < i + 1; ++j) {
            int rowA = j;
            int colA = i - j;
            int rowB = i - j;
            int colB = N - j - 1;
            temp += matA[rowA * N + colA] * matB[rowB * N + colB];
        }
        output[i] = temp;
      }
    }
    else
    {
    for(int i = start; i < end && i<2*N-1; ++i) {
        int temp = 0;
        for(int j = 0; j < 2 * N - (i + 1); ++j) {
            int rowA = i + 1 + j - N;
            int colA = N - j - 1;
            int rowB = N - j - 1;
            int colB = 2 * N - j - 2 - i;
            temp += matA[rowA * N + colA] * matB[rowB * N + colB];
        }
        output[i] = temp;
    }
    }
    return args;
}
void multiThread(int N, int *matA, int *matB, int *output)
{
    pthread_t T[max_thread];
    arguments p[max_thread];
    
    for(int i=0;i<max_thread;i++)
    {
    	p[i].mA=matA;
    	p[i].mB=matB;
    	p[i].mO=output;
    	p[i].id=i;
    	p[i].N=N;
    	pthread_create(&T[i],NULL,multiT,(void*)(&p[i]));
    }
    for(int i=0;i<max_thread;i++)
    {
    pthread_join(T[i],NULL);
    }
    
    
}

#include<cuda.h>
#include<cuda_runtime.h>

// Create other necessary functions here
__global__ void DMM(int *devA,int *devB,int *devO,int N)
{
	int i=threadIdx.x + blockDim.x*blockIdx.x;
	if(i<N)
    {
  
        int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; ++j) {
            int rowA = j;
            int colA = i - j;
            int rowB = i - j;
            int colB = N - j - 1;
            temp += devA[rowA * N + colA] * devB[rowB * N + colB];
        }
        devO[i] = temp;
    }
    else if(i<(2*N-1))
    	{

        int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * N - (i + 1); ++j) {
            int rowA = i + 1 + j - N;
            int colA = N - j - 1;
            int rowB = N - j - 1;
            int colB = 2 * N - j - 2 - i;
            temp += devA[rowA * N + colA] * devB[rowB * N + colB];
        }
        devO[i] = temp;
    }
	
}

// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{
	int *devA = NULL,*devB=NULL,*devO=NULL;
	int size1 =N*N*sizeof(int);
	int size2= (2*N-1)*sizeof(int);
	cudaError_t e = cudaSuccess;
	
	int threadperblock=64;
	int blockpergrid=(2*N-1+63)/threadperblock;
	
	//allocating space in device memory
	
	e= cudaMalloc((void **)&devA,size1);
	if(e != cudaSuccess)
	{
		fprintf(stderr,"failed to allocate memory for  matrix A  in device",cudaGetErrorString(e));
		exit(EXIT_FAILURE);
	}
	
	e= cudaMalloc((void **)&devB,size1);
	if(e != cudaSuccess)
	{
		fprintf(stderr,"failed to allocate memory for  matrix B  in device",cudaGetErrorString(e));
		exit(EXIT_FAILURE);
	}
	
	e= cudaMalloc((void **)&devO,size2);
	if(e != cudaSuccess)
	{
		fprintf(stderr,"failed to allocate memory for  matrix B  in device",cudaGetErrorString(e));
		exit(EXIT_FAILURE);
	}
	
	//transfer data from Host to device memory
	
	e=cudaMemcpy(devA,matA,size1,cudaMemcpyHostToDevice);
	
	if(e!=cudaSuccess)
	{
		fprintf(stderr,"copying failed",cudaGetErrorString(e));
		exit(EXIT_FAILURE);
	}
	
	e=cudaMemcpy(devB,matB,size1,cudaMemcpyHostToDevice);
	
	if(e!=cudaSuccess)
	{
		fprintf(stderr,"copying failed",cudaGetErrorString(e));
		exit(EXIT_FAILURE);
	}
	
	//launching kernel
	
	DMM<<<blockpergrid,threadperblock>>>(devA,devB,devO,N);
	
	cudaDeviceSynchronize();
	
	//transfer output from device to host
	
	e=cudaMemcpy(output,devO,size2,cudaMemcpyDeviceToHost);
	
	if(e!=cudaSuccess)
	{
		fprintf(stderr,"copying failed",cudaGetErrorString(e));
		exit(EXIT_FAILURE);
	}
	
	cudaFree(devA);
	cudaFree(devB);
	cudaFree(devO);
	
}

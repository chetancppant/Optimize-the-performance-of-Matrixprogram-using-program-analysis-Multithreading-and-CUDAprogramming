/*
// Optimize this function
void singleThread(int N, int *matA, int *matB, int *output)
{
    // Iterate over first half of output elements
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
*/
void singleThread(int N, int *matA, int *matB, int *output){
	int blocksize = 16;
	int index_A11,index_A12,index_A21,index_A22,index_B11,index_B12,index_B21,index_B22,index_O;
	
	for(int i = 0; i < N; i += blocksize)
	{ 
		for(int j = 0; j < N ; j += blocksize)
		{
			__builtin_prefetch(&matA[(i+1)*N+j+1],0,1);
			__builtin_prefetch(&matA[(i+2)*N+j+1],0,1);
			
				
			__builtin_prefetch(&matB[(j+1)*N+N-(i+1)],0,1);
			__builtin_prefetch(&matB[(j+2)*N+N-(i+1)],0,1);			
					
			for(int a= i ; a < i+blocksize; a+=2)
			{
				for(int b = j; b < j+blocksize; b+=2)
				{
					
					index_A11=a*N+b;
					index_A21=(a+1)*N+b;
					index_A12=a*N+b+1;
					index_A22=(a+1)*N+b+1;
					
					index_B12=b*N+N-1-(a);
					index_B11=b*N+N-1-(a+1);
					index_B22=(b+1)*N+N-1-(a);
					index_B21=(b+1)*N+N-1-(a+1);
					

					output[a+b] += matA[index_A11] * matB[index_B12];
					output[a+b+1]+=matA[index_A12]*matB[index_B22];
					output[a+1+b]+=matA[index_A21]*matB[index_B11];
					output[a+1+b+1]+=matA[index_A22]*matB[index_B21];
					
				}
			}
		}
	}
}


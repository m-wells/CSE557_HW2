#include <stdio.h>
#include <omp.h>
#define SIZE 8000	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "../../help_func.h"
#define NUMTHREADS 7

double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE];

void omp_triad_block(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE])
{
  int BLOCKSIZE = SIZE/250;

	#pragma omp parallel for
  for(int bi=0; bi<250; bi+=BLOCKSIZE) {                      
    for(int bj=0; bj<250; bj+=BLOCKSIZE) {                  
      for(int bk=0; bk<250; bk+=BLOCKSIZE) { 
	for (int i = 0; i < BLOCKSIZE; i++) {
		for (int j = 0; j < BLOCKSIZE; j++) {
			for (int k = 0; k < BLOCKSIZE; k++) {
				C[bi+i][bj+j] += A[bi+i][bk+k] * B[bk+k][bj+j];
				//printf("%d\n",omp_get_thread_num());
			}
		}
	}
      }
    }
  }
}
      
    

int main(int argc, const char *argv[])
{
	omp_set_num_threads(NUMTHREADS);
	/*
	double A[SIZE][SIZE] = {1,2,3,4,5,6,7,8,9};
	double B[SIZE][SIZE] = {3,2,3,4,5,6,7,8,9};
	double C[SIZE][SIZE] = {1,10,3,4,5,6,7,8,9};
	*/

	fill_random2d_double_seed(A,42);
	fill_random2d_double_seed(B,42);
	fill_random2d_double_seed(C,42);

	double start_time = omp_get_wtime();
	omp_triad_block(A,B,C);
	double end_time = omp_get_wtime() - start_time;

	//print_array2d_double('C',C);

	printf("NUMTHREADS:%d\n",NUMTHREADS);
	printf("unoptimized omp time: %f\n",end_time);
	double gflops = 2.*SIZE*SIZE*SIZE/(end_time * 1000000000.);
	printf("Unomptimized OMP GFLOP/s = %f\n",gflops);

	/*
	#pragma omp parallel
	{
		printf("omp_get_num_threads=%d\n",omp_get_num_threads());
	}
	*/

	return 0;
}

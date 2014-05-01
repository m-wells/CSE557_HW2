#include <stdio.h>
#include <omp.h>
#include <math.h>
#define SIZE 8000	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "../../help_func.h"
#define NUMTHREADS 3

double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE];

void omp_triad_block(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE])
{
  int STEPSIZE = 250;
    int BLOCKSIZE = SIZE/STEPSIZE;

	#pragma omp parallel for
  for(int bi=0; bi<BLOCKSIZE; bi++) {                      
    for(int bj=0; bj<BLOCKSIZE; bj++) {                  
      for(int bk=0; bk<BLOCKSIZE; bk++) { 
	for (int i = bi; i <fmin(bi+STEPSIZE-1,SIZE); i++) {
	  for (int j = bj; j <fmin(bj+STEPSIZE-1,SIZE); j++) {
	    for (int k = bk; k <fmin(bk+STEPSIZE-1,SIZE); k++) {
				C[i][j] += A[i][k] * B[k][j];
				//printf("%d\n",BLOCKSIZE);
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

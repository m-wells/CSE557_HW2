#include <stdio.h>
#include <omp.h>
#include <gmmintrin.h>
#define SIZE 8000	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "../help_func.h"
//#include <tmmintrin.h>

#define NUMTHREADS 1

double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE];




void omp_triad_vec(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE])
{
	#pragma omp parallel for
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE/4; k+=4) {
			  __m256d a = _mm_load_pd(&A[i]);
			  __m256d b = _mm_load_pd(&B[i]);
			  __m256d c = _mm_load_pd(&C[i][j]);
			  v = _mm_add_pd(_mm_mul_pd(a,b),c);
			  _mm_store_pd(&C[i][j],v);
				//printf("%d\n",omp_get_thread_num());
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
	omp_triad_vec(A,B,C);
	double end_time = omp_get_wtime() - start_time;

	//print_array2d_double('C',C);

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
